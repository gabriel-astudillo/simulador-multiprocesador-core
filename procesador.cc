#include "glob.h"
#include "procesador.h"
#include "tarea.h"


Procesador::Procesador(const string& _name, uint32_t _procID, uint32_t _totalCores) : process(_name), Memoria() {
	size_L2 = g_size_L2; /* Cantidad máxima de datos en L2 */
	
	name       = _name;
	procID     = _procID;
	totalCores = _totalCores;
	
	esperandoPorCore = false;
	
	Memoria::configuraMemoria(L2, size_L2);
	
	asociarCores();
	
	g_registro->print(this->time(), name , 
		string("Memoria L2:") + this->verDatos());
	
}


void Procesador::inner_body(){
	Tarea *tarea;
	
	/* Activa los cores asociados al procesador */
	std::list< handle<coreSim> >::iterator indexCore;
	indexCore = cores.begin();
	while( indexCore != cores.end() ){
		(*indexCore)->activate();
		indexCore++;
	}
	
	while(1){
		
		if( filaTareas.empty() ){
			g_registro->print(this->time(), name, "Fila vacía, procesador idle");
			esperandoPorCore = false;
			passivate();
		}
		
		tarea = filaTareas.front();
		filaTareas.pop_front();
		
		g_registro->print(this->time(), name, 
			string("INICIO Asignación tarea id:") + string(std::to_string(tarea->getID())) + string(", tServicio:" ) + string(std::to_string(tarea->getTservicio()))
				);
		
		/* Aquí se debe enviar la tarea a los core que tiene
		   asociado el procesador 
		   Lo anterior significa que el core NO TIENE 
		   una lista de tareas pendientes. Sólo ejecuta
		   una y espera la siguiente
		*/
		/*
			Ciclo de asignación de tareas
		*/
		std::list< handle<coreSim> >::iterator indexCore;
		while(1){
			
			/* 
			   Ciclo de revisión de cores.
			   Revisar cada core. Si no tiene una tarea asignada,
			   se la asigna una.
			*/
			indexCore = cores.begin();
			while( indexCore != cores.end() && tarea != NULL){
				if( ! (*indexCore)->tieneTareaAsignada() ){
					esperandoPorCore = false;
					(*indexCore)->agregarTarea(tarea);	
				
					if( (*indexCore)->idle() ){
						g_registro->print(this->time(), name, 
							string("FIN Asignación tarea id:") + string(std::to_string(tarea->getID())) + string(" --> ") + (*indexCore)->getName()
							);						
						(*indexCore)->activate();
					}
				
					/* La tarea ya está asignada, hay que 
					   salir del ciclo de revisión de cores 
					*/
					tarea = NULL;
				}
				
				indexCore++;
			} //FIN ciclo de revisión de cores
			
			/*
				Si la tarea no fue asignada (todos los cores tienen una),
			    entonces el procesador debe esperar hasta que uno se 
				libere. 
				En caso contrario, debe salir del ciclo de asignación.
			*/
			if( tarea != NULL ){
				esperandoPorCore = true;
				g_registro->print(this->time(), name, 
					string("Esperando por core para tarea id:") + string(std::to_string(tarea->getID() ))
					);
				passivate();
			}
			else{
				break;
			}
			
		} //FIN Ciclo de asignación de tareas	
		
	}
	
}

void Procesador::agregarTarea(Tarea* tarea){
	filaTareas.push_back(tarea);
	
	if( this->idle() && !this->estaEsperandoPorCore() ){
				g_registro->print(this->time(), name, "Activando Procesador");
				this->activate();
	}
}

void Procesador::asociarCores(){
	for(uint32_t i=0; i < totalCores; i++){
		coreSim *core;
		
		core = new coreSim(string("Core") + string(std::to_string(i)) , i);
		core->asociarProcesador(this);
		
		cores.push_back(core);
	}
}

bool Procesador::filaTareasEstaVacia(){
	return( filaTareas.empty() );
}

bool Procesador::estaEsperandoPorCore(){
	return( esperandoPorCore );
}

string Procesador::getName(){
	return(name);
}

uint32_t Procesador::getID(){
	return(procID);
}

uint32_t Procesador::getTotalCores(){
	return(totalCores);
}

Procesador::~Procesador(){
	
}


///////////////////////////


coreSim::coreSim(const string& _name, uint32_t _coreID) : process(_name), Memoria() {	
	size_L1 = g_size_L1; /* Cantidad máxima de datos en L1 */
	name = _name;
	coreID = _coreID;
	
	
	Memoria::configuraMemoria(L1, size_L1);
	
}


void coreSim::inner_body(){
	uint32_t index;
	
	index = procesador->getID()*procesador->getTotalCores() + coreID;
	
	g_registro->print(this->time(), this->getFullName() , 
		string("Memoria L1:") + this->verDatos());
	
	while(1){
		double tIni, tFin;
		
		if(tarea == NULL){
			
			g_registro->print(this->time(), this->getFullName(), "Sin tarea, idle");
			tIni = this->time();
			passivate();
			tFin = this->time();
			
			g_tiempoReposoCore[index] += tFin - tIni;
		}
		
		g_registro->print(this->time(), this->getFullName(), \
			string("INICIO tarea id:") +     \
			string( std::to_string(tarea->getID())) + \
			string(", tServicio:") +  \
			string(std::to_string(tarea->getTservicio()) ));
			
		
		/*==Simular procesamiento de la tarea==*/
		/*
			Cada tarea tiene cierta cantidad de datos que procesar.
				==> Hay que recorrer la lista de datos de la tarea
			Suposición: 
				El tiempo de servicio de la tarea es lo que se
				demora cada dato en procesar
		*/	
		tarea->setTInicioServicio(this->time());
		
		while( !tarea->datos.empty() ){
			char datoProcesar;
			t_dataStatus dataStatus_L1, dataStatus_L2;
			
			datoProcesar = tarea->datos.front();
			tarea->datos.pop_front();
			
			g_registro->print(this->time(), this->getFullName(), \
				string("PROCESAMIENTO tarea id:") + \
				string( std::to_string(tarea->getID())) + \
				string(", Dato:") + \
				datoProcesar \
			);
				
			// Revisar si datoProcesar está en L1						
			dataStatus_L1 = this->buscarDato(datoProcesar);
			
			if( dataStatus_L1 == DATA_FAIL ){
				/* 'datoProcesar' no está en memoria L1 */
				g_registro->print(this->time(), this->getFullName(), \
					string("PROCESAMIENTO tarea id:") + \
					string( std::to_string(tarea->getID())) + \
					string(", Dato:") + \
					datoProcesar + \
					string(" DATA_FAIL en L1") \
				);
				
				// Revisar si datoProcesar está en L2
				// Buscar el dato en L2 del procesador 
				// que contiene a este core
				dataStatus_L2 = procesador->buscarDato(datoProcesar);
				if( dataStatus_L2 == DATA_FAIL ){
					/* 'datoProcesar' no está en memoria L2 */
					g_registro->print(this->time(), this->getFullName(), \
						string("PROCESAMIENTO tarea id:") + \
						string( std::to_string(tarea->getID())) + \
						string(", Dato:") + \
						datoProcesar + \
						string(" DATA_FAIL en L2") \
					);
					//Transferir desde RAM hacia L2 (hay costo de transferencia) 				
					hold( TR_RAM_L2 );
					g_tiempoReposoCore[index] += TR_RAM_L2;
					procesador->ponerDato(datoProcesar);
					
					g_registro->print(this->time(), this->getFullName(), \
						string("PROCESAMIENTO tarea id:") + \
						string( std::to_string(tarea->getID())) + \
						string(", Dato:") + \
						datoProcesar + \
						string(" copiado a L2") \
					);
					g_registro->print(this->time(), this->getFullName(), 
						string("Memoria L2:") + procesador->verDatos());
						
					//Transferir desde L2 a L1 (hay costo de transferencia)					
					hold( TR_L2_L1 );	
					g_tiempoReposoCore[index] += TR_L2_L1;
					this->ponerDato(datoProcesar);	
					
					g_registro->print(this->time(), this->getFullName(), \
						string("PROCESAMIENTO tarea id:") + \
						string( std::to_string(tarea->getID())) + \
						string(", Dato:") + \
						datoProcesar + \
						string(" copiado a L1") \
					);
					g_registro->print(this->time(), this->getFullName(), 
						string("Memoria L1:") + this->verDatos());
							
				}
				else{
					/* 'datoProcesar' sí está en memoria L2 */
					g_registro->print(this->time(), this->getFullName(), \
						string("PROCESAMIENTO tarea id:") + \
						string( std::to_string(tarea->getID())) + \
						string(", Dato:") + \
						datoProcesar + \
						string(" DATA_OK en L2") \
					);
					//Transferir desde L2 a L1 (hay costo de transferencia)			
					hold( TR_L2_L1 );
					g_tiempoReposoCore[index] += TR_L2_L1;
					this->ponerDato(datoProcesar);	
					
					g_registro->print(this->time(), this->getFullName(), \
						string("PROCESAMIENTO tarea id:") + \
						string( std::to_string(tarea->getID())) + \
						string(", Dato:") + \
						datoProcesar + \
						string(" copiado a L1") \
					);
					g_registro->print(this->time(), this->getFullName(), 
						string("Memoria L1:") + this->verDatos());
								
				}									
			}
			
			/* 
			*   'datoProcesar' sí está en memoria L1 
			*   se puede procesar  
			*/
			g_registro->print(this->time(), this->getFullName(), \
				string("PROCESAMIENTO tarea id:") + \
				string( std::to_string(tarea->getID())) + \
				string(", Dato:") + \
				datoProcesar + \
				string(" DATA_OK en L1") \
			);

			hold( tarea->getTservicio() );	

		}
			
		tarea->setTFinServicio(this->time());
		
		double deltaTimeServicio = tarea->getTFinServicio() - tarea->getTInicioServicio();
		double deltaTimeEsperaReady = tarea->getTInicioServicio() - tarea->getTCreacion();
		
		g_registro->print(this->time(), this->getFullName(), \
			string("FIN    tarea id:") +  \
			string(std::to_string(tarea->getID()) ) + \
			string(" TservicioReal:") + \
			string( std::to_string(deltaTimeServicio) ) + \
			string(" , TesperaReady:") + \
			string( std::to_string(deltaTimeEsperaReady) ) \
		);
		
		
		g_tiempoServicio->update( deltaTimeServicio );
		
		g_tiempoServicioCore[ index ]->update( deltaTimeServicio );
		
		g_hist_tiempoServicio->update( deltaTimeServicio );
		
		g_tiempoEsperaReady->update( deltaTimeEsperaReady );
		g_tiempoEsperaReadyProc[procesador->getID()]->update( deltaTimeEsperaReady );
		
		g_hist_tiempoEsperaReady->update( deltaTimeEsperaReady );
		
		g_tareasFinalizadas->update(1.0);
		
		g_tput->update( g_tareasFinalizadas->value() / this->time() );
		
		//g_tiempoUtilizadoCore[index] += tarea->getTservicio() ; //REVISAR
		g_tiempoUtilizadoCore[index] += deltaTimeServicio ; 
		
		tarea = NULL;
		
		//ACTIVAR EL PROCESADOR SI ESTA IDLE
		//A CAUSA DE ESTAR ESPERANDO UN CORE LIBRE
		if( procesador->idle() && procesador->estaEsperandoPorCore() ){
			g_registro->print(this->time(), this->getFullName(), "Activando Procesador");
			procesador->activate();
		}
	}
	
}

void coreSim::agregarTarea(Tarea* _tarea){
	tarea = _tarea;
}

void coreSim::asociarProcesador(handle<Procesador> _procesador){
	procesador = _procesador;
}

bool coreSim::tieneTareaAsignada(){
	if(tarea == NULL){
		return(false);
	}
	else{
		return(true);
	}
}

string coreSim::getName(){
	return(name);
}

string coreSim::getFullName(){
	return(procesador->getName() + getName());
}

uint32_t coreSim::getID(){
	return(coreID);
}

coreSim::~coreSim(){
	
}



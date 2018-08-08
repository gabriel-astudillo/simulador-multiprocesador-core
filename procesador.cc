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
	
	g_registro->print(this->time(), name , 
		string("Memoria L2:") + this->verDatos());
		
	asociarCores();
	
}


void Procesador::inner_body(){
	Tarea *tarea;
	
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



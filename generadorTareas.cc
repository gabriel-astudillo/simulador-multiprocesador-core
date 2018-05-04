#include "glob.h"
#include "generadorTareas.h"
#include "procesador.h"
#include "tarea.h"

generadorTareas::generadorTareas(const string& _name, uint32_t _totalTareas, double _tasaLlegada, uint32_t _totalProcesadores, uint32_t _totalCores) : process(_name) {
	double lambda = 1.0/_tasaLlegada;
	
	tiempoArribo = new rngExp("Tiempo de arribo", lambda);
	tiempoArribo->reset();
	
	totalTareas       = _totalTareas;
	totalProcesadores = _totalProcesadores;
	totalCores        = _totalCores;
	
	name        = _name;
	
	/*Arreglo de procesadores*/
		
	arr_procesador= new handle<Procesador>[totalProcesadores];
	
	for(uint32_t i = 0; i < totalProcesadores; i++){
		arr_procesador[i] = new Procesador( string("Proc") + string(std::to_string(i)) , i, totalCores);
	}
	
}


void generadorTareas::inner_body(){
	Tarea *tarea;
	double tHold;
	uint32_t procID;
	
	/* Activar procesadores */
	for(uint32_t i = 0; i < totalProcesadores; i++){
		arr_procesador[i]->activate();
	}
	
	for(uint32_t tareaID=0; tareaID < totalTareas; tareaID+=1){
		
		tarea = new Tarea( tareaID, this->time() );

		g_registro->print(this->time(), \
			name, \
			string("Arribo Tarea id:")+string(std::to_string(tareaID)) + \
			string(" Datos:") + tarea->getDatos() );
		
		
		//procesador->agregarTarea(tarea);
		procID = tareaID % totalProcesadores; //0;
		arr_procesador[procID]->agregarTarea(tarea);
		
			
		tHold = tiempoArribo->value();		
		hold(tHold);
	}
	
}

generadorTareas::~generadorTareas(){
	
}


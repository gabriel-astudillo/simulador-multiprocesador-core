#include "glob.h"
#include "generadorTareas.h"
#include "procesador.h"
#include "tarea.h"

generadorTareas::generadorTareas(const string& _name, uint32_t _totalTareas, 
                                 double _tasaLlegada) :  process(_name) {
	double lambda = 1.0/_tasaLlegada;	
	totalTareas   = _totalTareas;
	name          = _name;	
	
	tiempoArribo = new rngExp("Tiempo de arribo", lambda);
	tiempoArribo->reset();
}

void generadorTareas::asociarProcesadores(vector< handle<Procesador> > _vector_procesadores){
	vector_procesadores = _vector_procesadores;
}

void generadorTareas::inner_body(){
	Tarea *tarea;
	double tHold;
	uint32_t procID;
	
	for(uint32_t tareaID=0; tareaID < totalTareas; tareaID+=1){
		
		tarea = new Tarea( tareaID, this->time() );

		g_registro->print(this->time(), \
			name, \
			string("Arribo Tarea id:")+string(std::to_string(tareaID)) + \
			string(" Datos:") + tarea->getDatos() );
		
		
		//Asignación RR de tareas a los procesadores
		procID = tareaID % vector_procesadores.size();
		vector_procesadores[procID]->agregarTarea(tarea);
		
			
		tHold = tiempoArribo->value();		
		hold(tHold);
	}
	
	
}

generadorTareas::~generadorTareas(){
	
}


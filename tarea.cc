#include "glob.h"
#include "tarea.h"

Tarea::Tarea(uint32_t _id, double _tiempoCreacion){
	
		id = _id;
		
		tiempoCreacion       = _tiempoCreacion;
		tiempoInicioServicio = 0.0;
		tiempoFinServicio    = 0.0;
		
		std::default_random_engine gb_random(id*10);
		//gb_random.seed(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_real_distribution<> uniform(5.33, 10.44);
		
		tServicio = uniform(gb_random);	
		
		asignarDatos();
		
}


void Tarea::asignarDatos(){
	for(uint8_t i=0; i < 1+rand()%10; i++){
		agregarDato(65+rand()%10);
	}
}

void Tarea::agregarDato(char dato){
	datos.push_back(dato);
}

	
uint32_t Tarea::getID(){
	return(id);
}

double Tarea::getTservicio(){
	return(tServicio);
}


std::string Tarea::getDatos(){
	list<char>::iterator indexDato;
	std::string *datos_tmp = new std::string();
	
	indexDato = datos.begin();
	
	while( indexDato != datos.end() ){
		datos_tmp->resize( datos_tmp->size()+1,  *indexDato);
		indexDato++;
	}
	return(*datos_tmp);
}

void Tarea::setTInicioServicio(double tiempoInicio){
	tiempoInicioServicio = tiempoInicio;
}
void Tarea::setTFinServicio(double finServicio){
	tiempoFinServicio = finServicio;
}

double Tarea::getTCreacion(){
	return(tiempoCreacion);
}

double Tarea::getTInicioServicio(){
	return(tiempoInicioServicio);
}

double Tarea::getTFinServicio(){
	return(tiempoFinServicio);
}



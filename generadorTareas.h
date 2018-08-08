#ifndef _GENERADOR_TAREAS_H_
#define _GENERADOR_TAREAS_H_

#include "procesador.h"

class generadorTareas :  public process {
private:
	
	rng<double>* tiempoArribo;
	uint32_t     totalTareas;
	string       name;
	
	vector< handle<Procesador> > vector_procesadores;
	
protected:
	void inner_body();
	
public:
	generadorTareas(const string& _name, uint32_t totalTareas, double _tasaLlegada);
	void asociarProcesadores(vector< handle<Procesador> > vector_procesadores);
	virtual ~generadorTareas();
	
};


#endif


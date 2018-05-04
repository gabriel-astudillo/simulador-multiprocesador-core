#ifndef _GENERADOR_TAREAS_H_
#define _GENERADOR_TAREAS_H_

#include "procesador.h"

class generadorTareas : public process {
private:
	
	rng<double>* tiempoArribo;
	uint32_t     totalTareas;
	
	uint32_t     totalProcesadores;
	uint32_t     totalCores;
	
	string       name;
	
	handle<Procesador> * arr_procesador;
	
	
	Registro *registro;
	
protected:
	void inner_body();
	
public:
	generadorTareas(const string& _name, uint32_t totalTareas, double _tasaLlegada, uint32_t _totalProcesadores, uint32_t _totalCores);
	virtual ~generadorTareas();
	
};


#endif


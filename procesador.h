#ifndef _PROCESADOR_H_
#define _PROCESADOR_H_

#include "coreSim.h"
#include "tarea.h"
#include "memoria.h"


class coreSim;

class Procesador : public process, public Memoria {
private:
	string name;
	uint32_t procID;
	list<Tarea*> filaTareas; //Tareas a repartir entre los cores
	Registro *registro;
	 
	list< handle<coreSim> > cores;
	
	uint8_t size_L2; //Tamaño memoria cache L2
	
	uint32_t totalCores;
	bool esperandoPorCore;
	
	void asociarCores();
	
	
protected:
	void inner_body();
	
public:
	Procesador(const string& _name, uint32_t _procID, uint32_t _totalCores);	
	virtual ~Procesador();
	void agregarTarea(Tarea* tarea);
	bool filaTareasEstaVacia();
	bool estaEsperandoPorCore();
	
	uint32_t getID();
	virtual string getName();
	uint32_t getTotalCores();

};


#endif


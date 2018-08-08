#ifndef _CORESIM_H_
#define _CORESIM_H_

#include "tarea.h"
#include "memoria.h"
#include "procesador.h"


class Procesador;

class coreSim : public process, public Memoria{
private:
	uint8_t size_L1; //Tamaño memoria cache L1
	
	string name;
	uint32_t coreID;
	Registro *registro;
	
	handle<Procesador> procesador;
	Tarea *tarea;
	
protected:
	void inner_body();
	
public:
	coreSim(const string& _name, uint32_t _coreID);
	virtual ~coreSim();
	
	void agregarTarea(Tarea* _tarea);
	void asociarProcesador(handle<Procesador> _procesador);
	bool tieneTareaAsignada();
	
	uint32_t getID();
	string getName();
	string getFullName();

	
};

#endif
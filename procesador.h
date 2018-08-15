#ifndef _PROCESADOR_H_
#define _PROCESADOR_H_

#include "tarea.h"
#include "memoria.h"

class Procesador : public process, public Memoria {
private:
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
		coreSim(const string& _name, uint32_t _coreID, handle<Procesador> _procesador);
		virtual ~coreSim();
	
		void agregarTarea(Tarea* _tarea);
		bool tieneTareaAsignada();
	
		string getName();
		string getFullName();	
	};
	
private:
	string name;
	uint32_t procID;
	list<Tarea*> filaTareas; //Tareas a repartir entre los cores
	Registro *registro;
	 
	list< handle<coreSim> > cores;
	
	uint8_t size_L2; //Tamaño memoria cache L2
	uint32_t totalCores;
	bool esperandoPorCore;
	
	
protected:
	void inner_body();
	
public:
	Procesador(const string& _name, uint32_t _procID, uint32_t _totalCores);	
	virtual ~Procesador();
	void agregarTarea(Tarea* tarea);
	bool filaTareasEstaVacia();
	bool estaEsperandoPorCore();
	
	uint32_t getID();
	uint32_t getTotalCores();
	
	string getName();
	
};




#endif


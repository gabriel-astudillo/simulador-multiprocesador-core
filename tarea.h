#ifndef _TAREA_H_
#define _TAREA_H_

#include <glob.h>

class Tarea {
private:
	uint32_t id;
	double tServicio;
	
	double tiempoCreacion;
	double tiempoInicioServicio;
	double tiempoFinServicio;
	
	void asignarDatos();
	void agregarDato(char dato);
	
protected:
	
	
public:
	list<char> datos;
	
	Tarea(uint32_t _id, double _tiempoCreacion);

	uint32_t getID();
	double getTservicio();
	std::string getDatos();
	
	void setTInicioServicio(double tiempoInicio);
	void setTFinServicio(double finServicio);
	
	double getTCreacion();
	double getTInicioServicio();
	double getTFinServicio();
	
	
};



#endif


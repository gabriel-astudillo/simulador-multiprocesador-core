#ifndef _TAREA_H_
#define _TAREA_H_

#include <glob.h>

class Tarea {
private:
	uint32_t id;
	double tServicio;
	list<char> datos;
	
	double tiempoCreacion;
	double tiempoInicioServicio;
	double tiempoFinServicio;
	
	void asignarDatos();
	void agregarDato(char dato);
	
protected:
	
	
public:
	
	Tarea(uint32_t _id, double _tiempoCreacion);

	uint32_t getID();
	double getTservicio();
	std::string getDatos();
	
	void setTInicioServicio(double tiempoInicio);
	void setTFinServicio(double finServicio);
	
	bool quedanDatos();
	char getProxDato();
	
	double getTCreacion();
	double getTInicioServicio();
	double getTFinServicio();
	
	
};



#endif


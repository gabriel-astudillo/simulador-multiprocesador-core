#ifndef _MEMORIA_H_
#define _MEMORIA_H_

#include <glob.h>

class Memoria {
private:
	t_memoria tipo;
	uint8_t   capacidad;
	
	/* Posición de memoria para escribir datos */
	uint8_t   pos; 
	
	std::string *datosEnMemoria;
	
protected:
	
	
public:
	Memoria();
	Memoria(t_memoria _tipo, uint8_t _capacidad);
	
	void configuraMemoria(t_memoria _tipo, uint8_t _capacidad);
	
	std::string verDatos();
	void ponerDato(char dato);
	t_dataStatus buscarDato(char dato);
	
	
};



#endif


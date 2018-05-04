#include "glob.h"
#include "memoria.h"


Memoria::Memoria(){
	
}

Memoria::Memoria(t_memoria _tipo, uint8_t _capacidad){
	
	configuraMemoria( _tipo,  _capacidad);

}

void Memoria::configuraMemoria(t_memoria _tipo, uint8_t _capacidad){
	tipo      = _tipo;
	capacidad = _capacidad;
	
	datosEnMemoria = new std::string(capacidad, 'x');
}

std::string Memoria::verDatos(){
	return (*datosEnMemoria);
}

void Memoria::ponerDato(char datoNuevo){
	/*
	*	Si 'datoNuevo' está en memoria, no hace nada.
	*	Si 'datoNuevo' no está en memoria, reemplaza el dato
	*	que está en la dirección 'pos' por 'datoNuevo'
	*
	*	La dirección 'pos' está entre 0 y capacidad-1
	*/
	
	std::size_t datoEncontrado = datosEnMemoria->find_first_of(datoNuevo);
	
	if( datoEncontrado == std::string::npos ){
		/* 
		*  No se encontró el dato 
		*  Se almacena en la posición 'pos'
		*/
		(*datosEnMemoria)[pos] = datoNuevo;
		
		pos++;
		pos = pos%capacidad;		
	}
}

t_dataStatus Memoria::buscarDato(char datoBuscar){
	t_dataStatus dataStatus;
	
	std::size_t datoEncontrado = datosEnMemoria->find_first_of(datoBuscar);
	
	if( datoEncontrado == std::string::npos ){
		/*
			No se encontró datoBuscar
		*/
		dataStatus = DATA_FAIL;
	}
	else{
		dataStatus = DATA_OK;
	}
	
	return(dataStatus);
}


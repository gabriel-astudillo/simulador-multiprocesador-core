#include "glob.h"
#include "check_args.h"

/*
*	Clase Registro
*/
Registro::Registro(){
	tipo = REG_VERBOSE;
}

Registro::Registro(t_register _tipo){
	tipo = _tipo;
}

void Registro::print(double tiempo, string quien, string mensaje){
	if( tipo == REG_VERBOSE){
		std::cout << std::setw(TIME_WIDTH) << std::fixed << tiempo << 
			": " << quien << ": " << mensaje << std::endl;
	}
	
}

/*
*	Indicadores
*/
counter*   g_tareasFinalizadas;
counter**  g_tareasFinalizadasProc;  

mean*      g_tiempoEsperaReady;       /* Global*/
mean**     g_tiempoEsperaReadyProc;   /* Por procesador */

mean*      g_tiempoServicio;      /* Global   */
mean**     g_tiempoServicioCore;  /* Por Core */
    
mean*      g_tput;		/* Global */    
mean**     g_tputProc;	/* Por procesador */   

    

histogram* g_hist_tiempoServicio;
histogram* g_hist_tiempoEsperaReady;

double*    g_tiempoUtilizadoCore;
double*    g_tiempoReposoCore;   /*tiempo de espera debido a transferencia de datos entre memorias*/

/*
*	Tiempos de transferencia
*	entre las distintas memorias
*/
double TR_L2_L1;
double TR_RAM_L2;

/*
*	Tamaños L1 y L2
*	(Falta parametrizar con check_args)
*/
uint8_t g_size_L1;// = 20;
uint8_t g_size_L2;// = 40;


Registro* g_registro;





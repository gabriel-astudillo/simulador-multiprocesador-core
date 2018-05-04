#include "glob.h"
#include "check_args.h"

extern char *optarg;
extern int optind, opterr, optopt;

/*
 *		Funcion que imprime en stdout el uso
 *		del programa.
 */

struct args_t  parametros;

void print_usage(char* name){
	char opciones[] = "-t <nro de tareas> -p <procesadores> -c <cores> -s <tasa llegada> [-l] [-L] [-m] [-M] [-v]\n";

	char descripcion[] = "Descripción:\n"
	                     "\t-t   cantidad de Tareas a simular\n"
						 "\t-s   taSa de llegada: numero medio de tareas que llegan por unidad de tiempo\n"
						 "\t-p   cantidad de Procesadores\n"
						 "\t-c   cantidad de Cores del procesador\n"
						 "\t-l   Tiempo de transferencia de datos desde L2 a L1. Omision: 0.3\n"	
						 "\t-L   Tiempo de transferencia de datos desde RAM a L2. Omision: 0.5\n"	
						 "\t-m   Tamaño memoria L1. Omision: 20\n"	
						 "\t-M   Tamaño memoria L2. Omision: 40\n"						 
						 "\t-v   Muestra en pantalla el registro de la simulacion\n";

	printf("Uso: %s %s\n%s\n", name, opciones, descripcion);
}
/* FIN print_usage()*/

void check_args(int argc , char **argv){
	int opcion;

	std::string optString = "t:c:s:l:L:m:M:p:hv";

	parametros.totalTareas = 0;
	parametros.totalProcesadores = 0;
	parametros.totalCores  = 0;
	parametros.tasaLlegada = 0.0;
	parametros.t_registro  = REG_QUIET;
	parametros.tr_L2_L1    = 0.3; 
    parametros.tr_RAM_L2   = 0.5;
	parametros.size_L1     = 20; 
    parametros.size_L2     = 40;
	
	
	while ((opcion = getopt (argc, argv, optString.c_str())) != -1){
		switch (opcion) {
			case 't':
					parametros.totalTareas = atoi(optarg);
					break;
			case 'c':
					parametros.totalCores = atoi(optarg);
					break;
			case 'v':
					parametros.t_registro = REG_VERBOSE;
					break;
			case 's':
					parametros.tasaLlegada = atof(optarg);
					break;
			case 'l':
					parametros.tr_L2_L1 = atof(optarg);
					break;
			case 'L':
					parametros.tr_RAM_L2 = atof(optarg);
					break;
			case 'm':
					parametros.size_L1 = atoi(optarg);
					break;
			case 'M':
					parametros.size_L2 = atoi(optarg);
					break;
			case 'p':
					parametros.totalProcesadores = atoi(optarg);
					break;
			case 'h':
			default:
					print_usage(argv[0]);
					exit(EXIT_FAILURE);
		}
	}

	if ( parametros.totalTareas == 0 || parametros.totalCores == 0 ||
	     parametros.tasaLlegada == 0.0 || parametros.totalProcesadores == 0) {
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
}
/* FIN check_args()*/

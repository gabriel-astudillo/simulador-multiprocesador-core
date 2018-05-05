#include "glob.h"
#include "check_args.h"

#include "generadorTareas.h"
#include "procesador.h"

class sistema: public process {
	
private:
	double simLen;
	uint32_t totalTareas;
	uint32_t totalCores;
	uint32_t totalProcesadores;
	
	double tasaLlegada;
	
	handle<generadorTareas> generador;
	handle<Procesador>      procesador;
	
	
public:
	sistema( const string& _name, double _sl, uint32_t _totalTareas, uint32_t _totalProcesadores, uint32_t _totalCores, double _tasaLlegada) : process( _name ) {
			 
		simLen      = _sl;
		totalTareas = _totalTareas;
		tasaLlegada = _tasaLlegada;
		
		totalCores        = _totalCores;
		totalProcesadores = _totalProcesadores;	
	
	}

protected:
  	void inner_body( void );
};


void sistema::inner_body( void ){
	
	generador  = new generadorTareas("Gen01", totalTareas, tasaLlegada, totalProcesadores, totalCores);

	generador->activate();

	hold(simLen);
	generador->cancel();
	
	end_simulation();

}

int main( int argc, char* argv[] ){

	check_args(argc, argv);
	
	TR_L2_L1   = parametros.tr_L2_L1; //por omision: 0.30
	TR_RAM_L2  = parametros.tr_RAM_L2;//por omision: 0.50
	
	g_size_L1 = parametros.size_L1;   //por omision: 20
	g_size_L2 = parametros.size_L2;   //por omision: 40
	
	/*
	*	Indicadores
	*/
	g_tareasFinalizadas   = new counter("Tareas_finalizadas");
	g_tareasFinalizadasProc = new counter*[parametros.totalProcesadores];;  
	
	for(uint32_t p=0; p < parametros.totalProcesadores; p++){
		g_tareasFinalizadasProc[p] = new counter("Tareas_finalizadas"); //Arreglar el nombre
	}
	
	g_tiempoEsperaReady     = new mean("Espera_en_fila_Ready", .95);
	g_tiempoEsperaReadyProc = new mean*[parametros.totalProcesadores];
	
	for(uint32_t p=0; p < parametros.totalProcesadores; p++){
		g_tiempoEsperaReadyProc[p] = new mean("Espera_en_fila_Ready", .95); //Arreglar el nombre
	}
	
	g_tiempoServicio      = new mean("Tiempo_servicio", .95); 	
	g_tiempoServicioCore  = new mean*[parametros.totalProcesadores*parametros.totalCores];
	
	for(uint32_t p=0; p < parametros.totalProcesadores; p++){
		for(uint32_t c=0; c < parametros.totalCores; c++){
			g_tiempoServicioCore[p*parametros.totalCores + c] = new mean("Tiempo_servicio", .95); //Arreglar el nombre
		}
	}
		
	g_tput     = new mean("Troughput", .95);
	g_tputProc = new mean*[parametros.totalProcesadores];
	
	for(uint32_t p=0; p < parametros.totalProcesadores; p++){
		g_tputProc[p] = new mean("Troughput", .95); //Arreglar el nombre
	}
	
	g_hist_tiempoServicio    = new histogram("Tiempo Servicio", 0.0, 100.0, 20);
	g_hist_tiempoEsperaReady = new histogram("Tiempo Espera Ready", 0.0, 100.0, 20);
	
	g_tiempoUtilizadoCore = (double*) calloc(parametros.totalProcesadores*parametros.totalCores, sizeof(double) );
	g_tiempoReposoCore    = (double*) calloc(parametros.totalProcesadores*parametros.totalCores, sizeof(double) );;
	
	/*
	*	Objeto utilizado para realizar
	*	el log del sistema
	*/
	g_registro = new Registro(parametros.t_registro);
	
	
	std::cout << "======================Sistema==========================" << std::endl;
	std::cout << "\tProcesadores:\t\t" << std::to_string(parametros.totalProcesadores) << std::endl;
	std::cout << "\tCores:\t\t\t" << std::to_string(parametros.totalCores) << std::endl;
	std::cout << "\tL1 size:\t\t" << std::to_string(g_size_L1) << std::endl;
	std::cout << "\tL2 size:\t\t" << std::to_string(g_size_L2) << std::endl;
	std::cout << "\tLatencia L2->L1:\t"  << TR_L2_L1 << std::endl;
	std::cout << "\tLatencia RAM->L2:\t" << TR_RAM_L2 << std::endl;
	
	std::cout << "\tTareas:\t\t\t"<< parametros.totalTareas << std::endl;	
	std::cout << "\tTasa de llegada:\t" << parametros.tasaLlegada << std::endl;	
	

	
	simulation::instance()->begin_simulation( new sqsDll( ) );

	handle<sistema> system = new sistema("System main", 
										1e100,                       /*Tiempo total de simulación*/
										parametros.totalTareas,      /*Cantidad de tareas        */
										parametros.totalProcesadores,/*Cantidad de procesadores  */ 
										parametros.totalCores,       /*Cantidad de cores         */
										parametros.tasaLlegada
										) ;

	system->activate();

	simulation::instance()->run();

	simulation::instance()->end_simulation();
	
	
	uint32_t index;
	try{
		printf("\n====================Estadisticas=======================\n\n");
		
		puts("---------------Utilizacion Proc y Cores----------------");
		printf("%5s%5s%15s%15s%15s\n", "Proc", "Core", "t. Utilizado", "Reposo", "utilizacion");
		for(uint32_t p=0; p < parametros.totalProcesadores; p++){
			for(uint32_t c=0; c<parametros.totalCores; c++){
				index = p*parametros.totalCores + c;
				double utilizacion = g_tiempoUtilizadoCore[index] > 0 ? (1.0 - g_tiempoReposoCore[index]/g_tiempoUtilizadoCore[index]) : 0.0;
				printf("%5i%5i%15f%15f%15f\n", p, c, g_tiempoUtilizadoCore[index], g_tiempoReposoCore[index], utilizacion);		
			}
		}
		puts("-------------------------------------------------------");
		puts("");
		puts("-------------Tiempo de Servicio---------------");
		printf("%-15.15s%15s%15s\n", "", "Prom.", "desv." );
		printf("%-15.15s%15.2f%15.2f\n", "Global"    , g_tiempoServicio->m()   , sqrt(g_tiempoServicio->variance()) );
		
		printf("%-15.15s\n", "Detalle" );
		printf("Proc\tCore\n");
	
		for(uint32_t p=0; p < parametros.totalProcesadores; p++){
			for(uint32_t c=0; c < parametros.totalCores; c++){				
				index = p*parametros.totalCores + c;
				try{
					printf("%d\t%d\t%14.2f%14.2f\n", p, c, g_tiempoServicioCore[index]->m() \
										 				 , sqrt(g_tiempoServicioCore[index]->variance()) );
				} catch (runtime_error& e){
					try{
						printf("%d\t%d\t%14.2f%14.2s\n", p, c, g_tiempoServicioCore[index]->m(), "-");
					}catch (runtime_error& e){
						printf("%d\t%d\t%14.2s%14.2s\n", p, c, "-", "-");
					}

				}		
				
			}
		}
		puts("----------------------------------------------");
		
		puts("");
		
		puts("----------Tiempo de Espera en Ready-----------");
		printf("%-15.15s%15s%15s\n", "", "Prom.", "desv." );
		printf("%-15.15s%15.2f%14.2f\n", "Global", g_tiempoEsperaReady->m()
												 , sqrt(g_tiempoEsperaReady->variance()) );
		printf("%-15.15s\n", "Detalle" );
		printf("%5s\n", "Proc");
		for(uint32_t p=0; p < parametros.totalProcesadores; p++){
			try{
				printf("%5d\t\t%14.2f%14.2f\n", p, g_tiempoEsperaReadyProc[p]->m()
						 						 , sqrt(g_tiempoEsperaReadyProc[p]->variance()) );
			}catch(runtime_error& e){
				try{
					printf("%5d\t\t%14.2f%14.2s\n", p, g_tiempoEsperaReadyProc[p]->m(), "-" );
				}catch (runtime_error& e){
					printf("%5d\t\t%14.2s%14.2s\n", p, "-", "-" );
				}
			}
			
			
		}
		puts("----------------------------------------------");
		

		//g_hist_tiempoServicio->report();
		//puts("-----");
		
		//g_hist_tiempoEsperaReady->report();
		//puts("-----");
		
		puts("");
		puts("-----------------------Troughput-----------------------");
		printf("%-10.10s%5s%15s%15s\n", "", "Tareas", "Prom.", "desv." );
		printf("%-10.10s%5.0f%15.2f%14.2f\n", "Global", g_tareasFinalizadas->value(), g_tput->m()
												 , sqrt(g_tput->variance()) );
		printf("%-15.15s\n", "Detalle" );
		printf("%5s\n", "Proc");
		for(uint32_t p=0; p < parametros.totalProcesadores; p++){
			try{
				printf("%5d%10.0f%15.2f%14.2f\n", p, g_tareasFinalizadasProc[p]->value(), g_tputProc[p]->m()
						 						 , sqrt(g_tputProc[p]->variance()) );
			}catch(runtime_error& e){
				try{
					printf("%5d%10.0f%15.2f%14.2s\n", p, g_tareasFinalizadasProc[p]->value(), g_tputProc[p]->m(), "-" );
				}catch(runtime_error& e){
					printf("%5d%10.0f%15.2s%14.2s\n", p, g_tareasFinalizadasProc[p]->value(), "-", "-" );
				}
			}		
		}
		puts("-------------------------------------------------------");
		puts("");
		
		
	} catch (runtime_error& e){
		std::cerr << "Error: " << e.what() <<std::endl;
	}
	

	return(EXIT_SUCCESS);
}

#ifndef _CHECK_ARGS_H_
#define _CHECK_ARGS_H_

struct args_t {
	uint32_t   totalTareas;
	uint32_t   totalProcesadores;
	uint32_t   totalCores;
	double     tasaLlegada;
	t_register t_registro;
	double     tr_L2_L1;
	double     tr_RAM_L2;
	uint8_t    size_L1;
	uint8_t    size_L2;
};

extern struct args_t parametros;

void check_args(int argc , char **argv);

#endif

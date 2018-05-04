# simulador-multiprocesador-core

```
./sim 
Uso: ./sim -t <nro de tareas> -p <procesadores> -c <cores> -s <tasa llegada> [-l] [-L] [-m] [-M] [-v]

Descripción:
	-t   cantidad de Tareas a simular
	-s   taSa de llegada: numero medio de tareas que llegan por unidad de tiempo
	-p   cantidad de Procesadores
	-c   cantidad de Cores del procesador
	-l   Tiempo de transferencia de datos desde L2 a L1. Omision: 0.3
	-L   Tiempo de transferencia de datos desde RAM a L2. Omision: 0.5
	-m   Tamaño memoria L1. Omision: 20
	-M   Tamaño memoria L2. Omision: 40
	-v   Muestra en pantalla el registro de la simulacion
```
Hasta el momento, el software entrega utilización de Procesadores y Cores, Tiempos de Servicio y Tiempos de Espera en la fila de entrada de los procesadores. Por ejemplo:

```
======================Sistema==========================
	Procesadores:		2
	Cores:			4
	L1 size:		20
	L2 size:		40
	Latencia L2->L1:	0.3
	Latencia RAM->L2:	0.5
	Tareas:			800
	Tasa de llegada:	10

====================Estadisticas=======================

---------------Utilizacion Proc y Cores----------------
 Proc Core   t. Utilizado         Reposo    utilizacion
    0    0    2916.130430       4.000000       0.998628
    0    1    2927.004084       4.616754       0.998423
    0    2    2926.022431       5.175263       0.998231
    0    3    2917.397724       4.780978       0.998361
    1    0    2859.303915       4.512078       0.998422
    1    1    2835.508661       4.119511       0.998547
    1    2    2821.925037       4.675586       0.998343
    1    3    2828.419720       4.309644       0.998476
-------------------------------------------------------

-------------Tiempo de Servicio---------------
                         Prom.          desv.
Global                   28.79          15.19
Detalle        
Proc	Core
0	0	         27.25         14.96
0	1	         30.49         15.21
0	2	         30.48         15.19
0	3	         28.89         17.09
1	0	         29.18         15.19
1	1	         29.23         16.08
1	2	         28.22         13.56
1	3	         26.94         14.18
----------------------------------------------

----------Tiempo de Espera en Ready-----------
                         Prom.          desv.
Global                 1413.99        821.49
Detalle        
 Proc
    0		       1449.54        833.24
    1		       1378.43        809.04
----------------------------------------------

-----------------------Troughput-----------------------
           Name /    Obs /   lBound /   uBound
      Troughput /    800 /   0.2647 /   0.2672
-------------------------------------------------------
```


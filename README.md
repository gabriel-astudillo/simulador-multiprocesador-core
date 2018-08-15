# simulador-multiprocesador-core

El diagrama de alto nivel del sistema es el siguiente:

![diagrama](https://raw.githubusercontent.com/gabriel-astudillo/simulador-multiprocesador-core/master/docs/diagrama2.png)

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
Hasta el momento, el software entrega utilización de Procesadores y Cores, Tiempos de Servicio, Tiempos de Espera en la fila de entrada de los procesadores y Troughput. Por ejemplo:

```
======================Sistema==========================
	Procesadores:		2
	Cores:			4
	L1 size:		20
	L2 size:		40
	Latencia L2->L1:	0.3
	Latencia RAM->L2:	0.5
	Tareas:			8000
	Tasa de llegada:	10

====================Estadisticas=======================

------------------------------------------------
Tiempo Simulacion: 29277.9366
------------------------------------------------

---------------Utilizacion Proc y Cores----------------
 Proc Core   t. Utilizado         Reposo    utilizacion
    0    0   29244.161555       4.000000       0.999863
    0    1   29277.819859       4.500000       0.999846
    0    2   29243.674843       5.000000       0.999829
    0    3   29271.920400       4.500000       0.999846
    1    0   28694.684991       4.500000       0.999843
    1    1   28652.376696       4.000000       0.999860
    1    2   28668.864485       4.500000       0.999843
    1    3   28667.869651       4.000000       0.999860
-------------------------------------------------------

-------------Tiempo de Servicio---------------
                         Prom.          desv.
Global                   28.97          14.93
Detalle        
Proc	Core
0	0	         28.84         15.18
0	1	         29.13         14.33
0	2	         30.21         15.26
0	3	         28.90         15.40
1	0	         28.98         14.86
1	1	         29.00         14.88
1	2	         28.64         14.40
1	3	         28.08         15.04
----------------------------------------------

----------Tiempo de Espera en Ready-----------
                         Prom.          desv.
Global                14096.19       8136.56
Detalle        
 Proc
    0		      14250.09       8226.68
    1		      13942.28       8043.50
----------------------------------------------

-----------------------Troughput-----------------------
          Tareas          Prom.          desv.
Global     8000           0.27          0.01
Detalle        
 Proc
    0      4000           0.27          0.01
    1      4000           0.27          0.01
-------------------------------------------------------
```


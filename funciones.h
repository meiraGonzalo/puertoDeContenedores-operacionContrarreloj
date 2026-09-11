#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "pila_dinamica.h"
#include "cola_dinamica.h"
#include "lista_dinamica.h"
#include <time.h>

#define MAX_COUNT 150
#define ERROR_ARCHIVO 0
#define CONF_INVALIDA 0
#define EXITO 1
#define POS_INVALIDA -1
#define FIN_LLEGADA 5 //tiempo antes de que termine la jornada en el que no pueden llegar mas buques

typedef struct{
    int duracion_jornada;//duracion de la jornada completa en minutos
    int cantidad_muelles;
    int cant_zonas;
    int cap_pila; //capacidad maxima de la pila de cada zona
    int max_buques;
    int max_camiones;
    int max_cont_buque;
    int tiempo_descarga;//minutos consumidos para descargar un contenedor y almacenarlo en una zona
    int tiempo_reubi; //minutos consumidos para trasladar un contenedor desde el tope de una zona al tope de otra
    int tiempo_carga; // minutos consumidos para retirar un contenedor del tope de una zona y cargarlo en el camión que corresponda.
}tConfig;

int CargarConfiguracion(char *nomarch, tConfig *conf);
int GenerarRandom(int liminf, int limsup);
int GenerarSimulacion(char *sim, tConfig *conf);

#endif // FUNCIONES_H_INCLUDED

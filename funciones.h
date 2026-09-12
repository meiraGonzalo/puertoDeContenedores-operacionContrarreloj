#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "bibliotecas/tdaPilaDinamica/pila_dinamica.h"
#include "bibliotecas/tdaColaDinamica/cola_dinamica.h"
#include "bibliotecas/tdaListaDinamica/lista_dinamica.h"
#include <time.h>

#define MAX_COUNT 150
#define ERROR_ARCHIVO 0
#define CONF_INVALIDA 0
#define EXITO 1
#define POS_INVALIDA -1
#define FIN_LLEGADA 5 //tiempo antes de que termine la jornada en el que no pueden llegar mas buques
#define COLA_VACIA 0
#define PILA_LLENA 0
#define TAM_COD 30

typedef struct{
    char cod[TAM_COD];
    int tiempo;
    tCola contenedores;
}tBuque;

typedef struct{
    char codcamion[TAM_COD];
    char codcont[TAM_COD];
}tCamion;

typedef struct{
    int nro;
    tBuque asignado;
    int estado; //1 libre, 0 ocupado
}tMuelle;

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
    int tiempo_carga; // minutos consumidos para retirar un contenedor del tope de una zona y cargarlo en el cami�n que corresponda.
}tConfig;

typedef struct{
    int nrozona;
    int cantcont;
    int maxcont;
    tPila pilacont;
}tZona;

int CargarConfiguracion(char *nomarch, tConfig *conf);
int GenerarRandom(int liminf, int limsup);
int GenerarSimulacion(char *sim, tConfig *conf, tCola *buques, tCola *camiones);
int InicializarZonas(tLista *zonas, int cant_zonas, int cap_pila);


#endif // FUNCIONES_H_INCLUDED

#ifndef GLOBALES_H_INCLUDED
#define GLOBALES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define FALLO 0
#define EXITO 1

#define MAX_COUNT 150
#define ERROR_ARCHIVO 0
#define CONF_INVALIDA 0
#define INSTR_INVALIDA 0
#define PARAM_INVALIDO -1
#define POS_INVALIDA -1
#define FIN_LLEGADA 20 //tiempo antes de que termine la jornada en el que no pueden llegar mas buques
#define COLA_VACIA 0
#define PILA_LLENA 0
#define PILA_VACIA 0
#define TAM_COD 30
#define TAM_CONSOLA 30 //el maximo de caracteres que seran captados cuando el usuario escriba algo por consola
#define VER_CAMIONES 3
#define NO_ENCONTRADO 0
#define MIN_CAMIONES 1
#define MIN_BUQUES 1
#define MIN_CONT_BUQUE 1


#include "../bibliotecas/tdaColaDinamica/cola_dinamica.h"
#include "../bibliotecas/tdaListaDinamica/lista_dinamica.h"
#include "../bibliotecas/tdaPilaDinamica/pila_dinamica.h"
typedef struct{
    char cod[TAM_COD];
    int tiempo;
    tCola contenedores;
}tBuque;

typedef struct{
    char codcamion[TAM_COD];
    char codcont[TAM_COD];
    int tiempo;
}tCamion;

typedef struct{
    int nro;
    tBuque asignado;
    int estado; //1 libre, 0 ocupado
}tMuelle;

typedef struct{
    int nrozona;
    int cantcont;
    int maxcont;
    tPila pilacont;
}tZona;

typedef struct
{
    int duracion_jornada;
    int cantidad_muelles;
    int cantidad_zonas;
    int capacidad_pila;
    int cant_max_buques;
    int cant_max_contenedores;
    int cant_max_camiones;
    int tiempo_descarga;
    int tiempo_reubicacion;
    int tiempo_carga;
} tConfig;

typedef struct
{
    int tiempo_actual;
    tCola buques_programados;
    tCola camiones_programados;
    tLista muelles;
    tLista zonas;
} tEstado;


#endif // GLOBALES_H_INCLUDED

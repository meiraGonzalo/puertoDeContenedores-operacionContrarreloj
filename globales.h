#ifndef GLOBALES_H_INCLUDED
#define GLOBALES_H_INCLUDED

#define MAX_COUNT 150
#define ERROR_ARCHIVO 0
#define CONF_INVALIDA 0
#define INSTR_INVALIDA 0
#define PARAM_INVALIDO -1
#define EXITO 1
#define POS_INVALIDA -1
#define FIN_LLEGADA 20 //tiempo antes de que termine la jornada en el que no pueden llegar mas buques
#define COLA_VACIA 0
#define PILA_LLENA 0
#define PILA_VACIA 0
#define TAM_COD 30
#define TAM_CONSOLA 30 //el maximo de caracteres que seran captados cuando el usuario escriba algo por consola
#define VER_CAMIONES 3
#define NO_ENCONTRADO 0
#define MIN_CAMIONES 6
#define MIN_BUQUES 6
#define MIN_CONT_BUQUE 2


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

#endif // GLOBALES_H_INCLUDED

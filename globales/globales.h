#ifndef GLOBALES_H_INCLUDED
#define GLOBALES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define FALLO 0
#define EXITO 1

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

#endif // GLOBALES_H_INCLUDED

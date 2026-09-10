#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED

#define NOMBRE_ARCHIVO_CONFIGURACION "config.txt"
#define FALLO 0
#define EXITO 1

typedef struct
{
    int duracion_jornada;
    int cantidad_muelles;
    int cantidad_zonas;
    int capacidad_pila;
    int cantidad_buques;
    int cantidad_contenedores;
    int cantidad_camiones;
    int tiempo_descarga;
    int tiempo_reubicacion;
    int tiempo_carga;
} tConfiguracion;

void configuracionIniciar (tConfiguracion* config);
int cargarConfiguracion (tConfiguracion* config);
int configuracionCompleta (const tConfiguracion* config);


#endif // CONFIGURACION_H_INCLUDED

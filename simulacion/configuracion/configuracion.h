#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED
#include "../../globales/globales.h"

#define NOMBRE_ARCHIVO_CONFIGURACION "config.txt"

void configuracionIniciar (tConfig* config); //inicializa los valores
int cargarConfiguracion (tConfig* config); //extrae los datos de config.txt
int configuracionCompleta (const tConfig* config); //chequea que este completa

#endif // CONFIGURACION_H_INCLUDED

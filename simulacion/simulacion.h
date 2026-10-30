#ifndef SIMULACION_H_INCLUDED
#define SIMULACION_H_INCLUDED

#include "../asignacion_buques/buque_muelle.h"
#include "../funciones.h"
#include "../globales/globales.h"
#include "configuracion/configuracion.h"
#include "../usuario/usuario.h"

int simulacion();
void estadoIniciar(tEstado* sistema);
void procesarEventos(tEstado* sis, tCola* buquesEspera, tCola* camionesEspera, tOperador* usuario);
int proximoEvento(tEstado* sis, int duracion);
int hayOperacionValida(tEstado* sis, tCola* camionesEspera, const tConfig* conf);
int finalizacionNormal(tEstado* sis, tCola* buquesEspera, tCola* camionesEspera, const tConfig* conf);
int bloqueoOperativo(tEstado* sis, tCola* buquesEspera, tCola* camionesEspera, const tConfig* conf);
int jornadaFinalizada(tEstado* sis, tCola* buquesEspera, tCola* camionesEspera, const tConfig* conf);
void avanzarRelojAuto(tEstado* sis, tCola* buquesEspera, tCola* camionesEspera, tOperador* usuario, const tConfig* conf);
void vaciarSimulacion (tEstado* sim, tCola* camionesEspera, tCola* buquesEspera);


#endif // SIMULACION_H_INCLUDED

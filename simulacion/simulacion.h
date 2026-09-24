#ifndef SIMULACION_H_INCLUDED
#define SIMULACION_H_INCLUDED

#include "../asignacion_buques/buque_muelle.h"
#include "../funciones.h"
#include "../globales/globales.h"
#include "configuracion/configuracion.h"
#include "../usuario/usuario.h"

int simulacion();
void estadoIniciar(tEstado* sistema);
int bloqueoOperativo (tEstado* sis, tCola* camionesEspera);

#endif // SIMULACION_H_INCLUDED

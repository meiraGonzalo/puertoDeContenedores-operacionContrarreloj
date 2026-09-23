#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "operaciones/operaciones.h"
#include "asignacion_buques/buque_muelle.h"
#include "bibliotecas/tdaPilaDinamica/pila_dinamica.h"
#include "bibliotecas/tdaColaDinamica/cola_dinamica.h"
#include "bibliotecas/tdaListaDinamica/lista_dinamica.h"
#include <time.h>





int CargarConfiguracion(char *nomarch, tConfig *conf);
int GenerarRandom(int liminf, int limsup);
int GenerarSimulacion(char *sim, tConfig *conf, tCola *buques, tCola *camiones);
int InicializarZonas(tLista *zonas, int cant_zonas, int cap_pila);
int ProcesarInstruccion(char *instruccion,tLista *muelles, tLista *zonas, tCola *camiones, int *tiempo, tConfig conf);
int ValidarRangoInt(int num, int liminf, int limsup);


#endif // FUNCIONES_H_INCLUDED

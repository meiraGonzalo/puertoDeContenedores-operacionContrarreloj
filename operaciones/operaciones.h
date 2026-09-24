#ifndef OP_H_INCLUDED
#define OP_H_INCLUDED

#include "../bibliotecas/tdaPilaDinamica/pila_dinamica.h"
#include "../bibliotecas/tdaColaDinamica/cola_dinamica.h"
#include "../bibliotecas/tdaListaDinamica/lista_dinamica.h"
#include "../bibliotecas/nodo/nodo.h"

#include "../globales/globales.h"

int Descargar(tLista *muelles, tLista *zonas, int nro_muelle, int nro_zona, FILE* log, int *tiempo, tConfig conf);
void VerZona(void *zona, void *sin_usar);
void VerCamiones(tCola *camiones, int cant, int tiempo_actual);
int Reubicar(tLista *zonas, int zona_origen, int zona_destino, tOperador* usuario, FILE* log, int *tiempo, tConfig conf);
int Entregar(tLista *zonas, tCola *camiones, tOperador* usuario, FILE* log, int *tiempo, tConfig conf);
#endif // OP_H_INCLUDED

#ifndef BUQUE_MUELLE_H_INCLUDED
#define BUQUE_MUELLE_H_INCLUDED

#include "../bibliotecas/tdaColaDinamica/cola_dinamica.h"
#include "../bibliotecas/tdaListaDinamica/lista_dinamica.h"
#include "../funciones.h"

void LiberarVacios(void *infoMuelle, void *usuario);
void EmbarcarBuquesVacios(tLista *muelles, tOperador* usuario);
int InicializarMuelles(tLista *muelles, int cant_muelles);
int AsignarBuques(tCola *buques, tLista *muelles, int tiempo);
void MostrarMuelle(void *muelle, void *sin_usar);

#endif

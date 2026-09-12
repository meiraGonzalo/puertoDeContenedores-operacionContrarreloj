#ifndef OP_H_INCLUDED
#define OP_H_INCLUDED

#include "../funciones.h"

int Descargar(tLista *muelles, tLista *zonas, int nro_muelle, int nro_zona);
void VerZona(void *zona, void *sin_usar);
void VerCamiones(tCola *camiones, int cant);
#endif // OP_H_INCLUDED

#ifndef PILAD_H_INCLUDED
#define PILAD_H_INCLUDED

#define MIN(X,Y) ((X<Y)?X:Y)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../nodo/nodo.h"

typedef tNodo* tPila;

void CreateStack(tPila *pila);
int PushStack(tPila *pila, void *info, size_t taminfo);
int PopStack(tPila *pila, void *info, size_t taminfo);
int PeekStack(tPila *pila, void *info, size_t taminfo);
int IsEmptyStack(tPila *pila);
#endif // PILAD_H_INCLUDED

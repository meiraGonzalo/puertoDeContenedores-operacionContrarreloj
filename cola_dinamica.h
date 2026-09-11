#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#define MIN(X,Y) ((X<Y)?X:Y)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nodo.h"

typedef struct{
    tNodo *pri;
    tNodo *ult;
}tCola;

void CreateQueue(tCola *pCola);
int EnQueue(tCola *pCola, void *info, size_t taminfo);
int DeQueue(tCola *pCola, void *info, size_t taminfo);
int ViewFirst(tCola *pCola, void *info, size_t taminfo);
int IsEmptyQueue(tCola *pCola);
#endif // COLA_H_INCLUDED

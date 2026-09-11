#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#define MIN(X,Y) ((X<Y)?X:Y)

#define ERROR_MALLOC 0
#define POS_INVALIDA -1
#define EXITO 1
#define LISTA_VACIA -1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodo.h"
typedef tNodo* tLista;

void CrearLista(tLista *pLista);
int InsPrinLista(tLista *pLista, void *info, size_t taminfo);
int OutPrinLista(tLista *pLista, void *info, size_t taminfo);
int InsUltLista(tLista *pLista, void *info, size_t taminfo);
int OutUltLista(tLista *pLista, void *info, size_t taminfo);
int IntPosLista(tLista *pLista, void *info, size_t taminfo,size_t pos);
int OutPosLista(tLista *pLista, void *info, size_t taminfo,size_t pos);
void RecorrerLista(tLista *pLista, void accion(void*, void*), void *param);
void ReduceLista(tLista *pLista, void reduce(void*, void*, void*), void *resu, void *param);
tLista *FilterLista(tLista *pLista, int filtro(void*, void*), void *param);
int BuscLista(tLista *pLista, void *info, int cmp(void*, void*));
tLista* BuscarMenor(tLista *pLista, int cmp(void*, void*));
int OrdenarLista(tLista *pLista, int cmp(void*, void*));
void VaciarLista(tLista *pLista);

#endif // LISTA_H_INCLUDED

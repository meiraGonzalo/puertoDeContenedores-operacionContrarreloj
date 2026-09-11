#include "pila_dinamica.h"


void CreateStack(tPila *pila){
    *pila=NULL;
}

int IsEmptyStack(tPila *pila){
    return (*pila==NULL)? 1:0;
}

int PushStack(tPila *pila, void *info, size_t taminfo){
    tNodo *nuevo;
    if(!(nuevo=malloc(sizeof(tNodo)))){
        return 0;
    }
    if(!(nuevo->info=malloc(taminfo))){
        free(nuevo);
        return 0;
    }
    memcpy(nuevo->info, info, taminfo);
    nuevo->taminfo=taminfo;
    nuevo->sig=*pila;
    *pila=nuevo;
    return 1;

}
int PopStack(tPila *pila, void *info, size_t taminfo){
    tNodo *aux;
    if(IsEmptyStack(pila)){
        return 0;
    }
    aux=*pila;
    memcpy(info, aux->info, MIN(aux->taminfo, taminfo));
    *pila=aux->sig;
    free(aux->info);
    free(aux);
    return 1;
}

int PeekStack(tPila *pila, void *info, size_t taminfo){
    if(IsEmptyStack(pila)){
        return 0;
    }
    memcpy(info, (*pila)->info, MIN((*pila)->taminfo, taminfo));
    return 1;
}


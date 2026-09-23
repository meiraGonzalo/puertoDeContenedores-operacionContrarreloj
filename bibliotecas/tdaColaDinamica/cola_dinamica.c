#include "cola_dinamica.h"

void CreateQueue(tCola *pCola){
    pCola->pri=NULL;
    pCola->ult=NULL;
}
int EnQueue(tCola *pCola, void *info, size_t taminfo){
    //creamos el nuevo nodo
    tNodo *nuevo;
    if(!(nuevo=malloc(sizeof(tNodo))))
        return 0;
    if(!(nuevo->info=malloc(taminfo))){
        free(nuevo);
        return 0;
    }
    //copiamos la info
    memcpy(nuevo->info, info, taminfo);
    nuevo->taminfo=taminfo;
    nuevo->sig=NULL;
    if(IsEmptyQueue(pCola))
        pCola->pri=nuevo;
    else
        pCola->ult->sig=nuevo;
    //siempre ult apunta al nodo insertado
    pCola->ult=nuevo;
    return 1;
}
int DeQueue(tCola *pCola, void *info, size_t taminfo){
    if(IsEmptyQueue(pCola)){
        return 0;
    }
    tNodo *aux;
    //aux contiene el nodo que sera eliminado
    aux= pCola->pri;
    //el primero pasa a ser el segundo en la cola
    pCola->pri=aux->sig;
    memcpy(info, aux->info, MIN(aux->taminfo,taminfo));
    free(aux->info);
    free(aux);
    if(pCola->pri==NULL){
        pCola->ult=NULL; // si despues de sacar no queda nadie en la cola, ult tambien pasa a ser NULL
    }
    return 1;
}
int ViewFirst(tCola *pCola, void *info, size_t taminfo){
    if(IsEmptyQueue(pCola)){
        return 0;
    }
    //copiamos el contenido de info
    memcpy(info, (pCola->pri)->info, MIN((pCola->pri)->taminfo,taminfo));
    return 1;
}
int IsEmptyQueue(tCola *pCola){
    return (pCola->pri)?0:1;
}

void VaciarCola(tCola *pCola){
    while(pCola->pri){
        tNodo *aux=pCola->pri;
        pCola->pri=aux->sig;
        free(aux->info);
        free(aux);
    }
    //una vez vaciado ult tambien pasa a ser null
    pCola->ult=NULL;
}

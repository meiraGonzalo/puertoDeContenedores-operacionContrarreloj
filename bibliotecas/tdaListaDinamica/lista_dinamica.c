#include "lista_dinamica.h"

void CrearLista(tLista *pLista){
    *pLista=NULL;
}
int InsPrinLista(tLista *pLista, void *info, size_t taminfo){
    tNodo *nuevo;
    if(!(nuevo=malloc(sizeof(tNodo))))
       return ERROR_MALLOC;
    if(!(nuevo->info=malloc(taminfo))){
        free(nuevo);
        return ERROR_MALLOC;
    }
    nuevo->taminfo=taminfo;
    memcpy(nuevo->info, info, taminfo);
    nuevo->sig=*pLista;
    *pLista=nuevo;
    return EXITO;
}
int OutPrinLista(tLista *pLista, void *info, size_t taminfo){
    if(!(*pLista)){
        return LISTA_VACIA;
    }
    tNodo *aux=*pLista;
    *pLista=aux->sig;
    memcpy(info, aux->info, MIN(taminfo, aux->taminfo));
    free(aux->info);
    free(aux);
    return EXITO;
}
int InsUltLista(tLista *pLista, void *info, size_t taminfo){
    while(*pLista){
        pLista=&(*pLista)->sig;
    }
    return InsPrinLista(pLista, info, taminfo);
}
int OutUltLista(tLista *pLista, void *info, size_t taminfo){
    if(!(*pLista))
        return LISTA_VACIA;
    while((*pLista)->sig){
        pLista=&(*pLista)->sig;
    }
    tNodo *aux=*pLista;
    *pLista=NULL;
    memcpy(info, aux->info, taminfo);
    free(aux->info);
    free(aux);
    return EXITO;

}
int InsPosLista(tLista *pLista, void *info, size_t taminfo,size_t pos){
    while(*pLista && pos){
        pLista=&(*pLista)->sig;
        pos--;
    }
    if(pos)
        return POS_INVALIDA;
    return InsPrinLista(pLista, info, taminfo);

}
int OutPosLista(tLista *pLista, void *info, size_t taminfo,size_t pos){
    tNodo *aux;
    while(*pLista && pos){
        pLista=&(*pLista)->sig;
        pos--;
    }
    if(pos || !*pLista)
        return POS_INVALIDA;
    aux=*pLista;
    *pLista=aux->sig;
    memcpy(info, aux->info, taminfo);
    free(aux->info);
    free(aux);
    return EXITO;
}

int VerPosLista(tLista *pLista, void *info, size_t taminfo,size_t pos){
    while(*pLista && pos){
        pLista=&(*pLista)->sig;
        pos--;
    }
    if(pos || !*pLista)
        return POS_INVALIDA;
    memcpy(info, (*pLista)->info, taminfo);
    return EXITO;
}
void RecorrerLista(tLista *pLista, void accion(void*, void*), void *param){
    while(*pLista){
        accion((*pLista)->info, param);
        pLista=&(*pLista)->sig;
    }
}
void ReduceLista(tLista *pLista, void reduce(void*, void*, void*), void *resu, void *param){
    while(*pLista){
        reduce((*pLista)->info, resu, param);
        pLista=&(*pLista)->sig;
    }
}
tLista *FilterLista(tLista *pLista, int filtro(void*, void*), void *param){
    tNodo *aux;
    tLista *inicio=pLista;
    while(*pLista){
        if(filtro((*pLista)->info, param)){
            pLista=&(*pLista)->sig;
        }
        else{
            aux=*pLista;
            *pLista=aux->sig;
            free(aux->info);
            free(aux);
        }
    }
    return inicio;
}
int BuscLista(tLista *pLista, void *info, int cmp(void*, void*)){
    size_t pos=0;
    while(*pLista){
        if(cmp((*pLista)->info, info)==0){
            return pos;
        }
        pLista=&(*pLista)->sig;
        pos++;
    }
    return POS_INVALIDA;
}

int OutDupliLista(tLista *pLista, int cmp(void*, void*)){
    int cant=0;
    if(!(*pLista))
        return LISTA_VACIA;
    while(*pLista){
        tNodo *anterior=*pLista;
        tNodo *revisar=anterior->sig;
        while(revisar){
            if(cmp((*pLista)->info, revisar->info)==0){ //son iguales
                anterior->sig=revisar->sig;
                free(revisar->info);
                free(revisar);
                revisar=anterior->sig;
                cant++;
            }
            else{
                anterior=revisar;
                revisar=revisar->sig;
            }
        }
        pLista=&(*pLista)->sig;
    }
    return cant;
}

tLista *BuscarMenor(tLista *pLista, int cmp(void*, void*)){
    tLista *menor= pLista;
    pLista=&(*pLista)->sig;
    while(*pLista){
        if(cmp((*menor)->info,(*pLista)->info)>0)
            menor=pLista;
        pLista=&(*pLista)->sig;
    }
    return menor;
}

int OrdenarLista(tLista *pLista, int cmp(void*, void*)){
     tNodo *aux, **menor;

    if(!*pLista){
        return LISTA_VACIA;
    }
    while(*pLista){
        menor=BuscarMenor(pLista, cmp);
        if(*menor!=*pLista){
            aux=*menor;
            *menor=aux->sig;
            aux->sig=*pLista;
            *pLista=aux;
        }
        pLista=&(*pLista)->sig;
     }
     return EXITO;
}

void VaciarLista(tLista *pLista){
    tNodo *elim;
    while(*pLista){
        elim=*pLista;
        *pLista=(*pLista)->sig;
        free(elim->info);
        free(elim);
    }
}

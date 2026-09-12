#include "operaciones.h"

int Descargar(tLista *muelles, tLista *zonas, int nro_muelle, int nro_zona){
    int posmuelle=nro_muelle-1,poszona=nro_zona-1;
    char codcont[TAM_COD];
    tZona zona;
    tMuelle muelle;
    tBuque buque;
    //verificamos que haya espacio en la zona
    OutPosLista(zonas, &zona, sizeof(tZona), poszona);
    if(zona.cantcont>=zona.maxcont)
        return PILA_LLENA;
    //sacamos el contenedor del buque
    OutPosLista(muelles,&muelle, sizeof(tMuelle),posmuelle);
    buque=muelle.asignado;
    DeQueue(&(buque.contenedores), codcont, sizeof(codcont)); //quitamos el contenedor del buque
    muelle.asignado=buque;
    InsPosLista(muelles, &muelle, sizeof(tMuelle), posmuelle);
    //ingresamos el contenedor
    PushStack(&(zona.pilacont), codcont, strlen(codcont)+1); //ponemos el contenedor en la pila de la zona
    zona.cantcont++;
    InsPosLista(zonas, &zona, sizeof(tZona),poszona);
    return EXITO;
}
void VerZona(void *info, void *sin_usar){ //hace falta crear una nueva pila y mover todos los elementos ahi para luego volver a poner todos los elementos devuelta en la original
    char codcont[TAM_COD];
    tPila pilaAux;
    CreateStack(&pilaAux);
    tZona *zona=(tZona*)info;

    printf("Z%d:[", zona->nrozona);
    while(PopStack(&(zona->pilacont), codcont, sizeof(codcont))){
        PushStack(&pilaAux, codcont, strlen(codcont)+1);
        printf("%s", codcont);
        if(PeekStack(&pilaAux,codcont,sizeof(codcont)))
            printf("|");
    }
    //restauramos pila original
    while(PopStack(&pilaAux, codcont, sizeof(codcont)))
        PushStack(&(zona->pilacont),codcont, strlen(codcont)+1);

    printf("]\t");
}


void VerCamiones(tCola *camiones, int cant){//hace falta crear una nueva cola y mover todos lo elementos ahi para luefo restaurar la original
    tCamion camion;
    tCola colaAux;
    CreateQueue(&colaAux);
    printf("Cola Camiones: ");
    if(!ViewFirst(camiones, &camion, sizeof(tCamion)))
       printf("vacia");
    while(DeQueue(camiones, &camion, sizeof(tCamion))){
        EnQueue(&colaAux, &camion,sizeof(tCamion));
        if(cant>0){
        printf("%s",camion.codcamion);
        if(ViewFirst(camiones,&camion, sizeof(tCamion)) && cant>1)
            printf(",");
        }
        cant--;
    }
    printf("\n");
    //restauramos la cola original
    while(DeQueue(&colaAux, &camion, sizeof(tCamion)))
        EnQueue(camiones, &camion, sizeof(tCamion));
}

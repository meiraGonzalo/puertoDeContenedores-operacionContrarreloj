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
        if(PeekStack(&(zona->pilacont),codcont,sizeof(codcont)))
            printf("|");
    }
    //restauramos pila original
    while(PopStack(&pilaAux, codcont, sizeof(codcont)))
        PushStack(&(zona->pilacont),codcont, strlen(codcont)+1);

    printf("]\t");
}


void VerCamiones(tCola *camiones, int cant, int tiempo_actual){//hace falta crear una nueva cola y mover todos lo elementos ahi para luefo restaurar la original
    tCamion camion;
    tCola colaAux;
    int i=1;
    CreateQueue(&colaAux);
    printf("Cola Camiones: ");
    if(!ViewFirst(camiones, &camion, sizeof(tCamion)))
       printf("vacia");
    printf("\n");
    while(DeQueue(camiones, &camion, sizeof(tCamion))){
        EnQueue(&colaAux, &camion,sizeof(tCamion));
        if(cant>0 && (camion.tiempo<=tiempo_actual)){
        printf("%d)%s --> %s\n",i,camion.codcamion, camion.codcont);
        }
        cant--;
        i++;
    }
    printf("\n");
    //restauramos la cola original
    while(DeQueue(&colaAux, &camion, sizeof(tCamion)))
        EnQueue(camiones, &camion, sizeof(tCamion));
}

int Reubicar(tLista *zonas, int nro_origen, int nro_destino, tOperador* usuario){
    char codcont[TAM_COD];
    int pos_origen=nro_origen-1, pos_destino=nro_destino-1;
    tZona zona_ori, zona_dest;
    //verificamos que la zona destino no este llena
    OutPosLista(zonas, &zona_dest, sizeof(tZona), pos_destino);
    if(zona_dest.cantcont>=zona_dest.maxcont)
        return PILA_LLENA;
    //quitamos el contenedor de la zona origen
    OutPosLista(zonas, &zona_ori,sizeof(tZona), pos_origen);
    if(zona_ori.cantcont==0)
        return PILA_VACIA;
    PopStack(&(zona_ori.pilacont), codcont, sizeof(codcont));
    zona_ori.cantcont--;
    InsPosLista(zonas, &zona_ori,sizeof(tZona), pos_origen);
    // lo ponemos en la zona destino
    PushStack(&(zona_dest.pilacont),codcont,strlen(codcont)+1);
    zona_dest.cantcont++;
    InsPosLista(zonas, &zona_dest, sizeof(tZona), pos_destino);
    usuario->cantReubicar++;
    return EXITO;
}

int CmpContTope(void *info_zona, void *info_codcont){
    char conttope[TAM_COD];
    char *codcont=(char*)info_codcont;
    tZona *zona=(tZona*)info_zona;
    if(IsEmptyStack(&(zona->pilacont))){
        return -1;
    }
    PeekStack(&(zona->pilacont), conttope, sizeof(conttope));
    return strcmpi(conttope, codcont);
}

int Entregar(tLista *zonas, tCola *camiones, tOperador* usuario){
    int pos_zona;
    char codcont[TAM_COD];
    tCamion camion;
    tZona zona;
    if(!ViewFirst(camiones, &camion, sizeof(camion)))
        return COLA_VACIA;
    pos_zona=BuscLista(zonas, camion.codcont, CmpContTope);
    if(pos_zona==-1)
        return NO_ENCONTRADO;
    DeQueue(camiones, &camion, sizeof(tCamion));
    OutPosLista(zonas, &zona, sizeof(tZona),pos_zona);
    PopStack(&(zona.pilacont), codcont, sizeof(codcont));
    InsPosLista(zonas, &zona, sizeof(tZona),pos_zona);
    printf("\ncontenedor %s entregado al camion %s\n", codcont, camion.codcamion);
    usuario->puntuacion += 10;
    usuario->contenEntregados++;
    return EXITO;

}

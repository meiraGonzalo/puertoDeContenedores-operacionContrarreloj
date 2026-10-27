#include "operaciones.h"

int Descargar(tLista *muelles, tLista *zonas, int nro_muelle, int nro_zona, FILE* log, int *tiempo, tConfig conf){
    char codcont[TAM_COD];
    tZona *zona;
    tMuelle *muelle;

    if(*tiempo+conf.tiempo_descarga > conf.duracion_jornada)
    {
        fprintf (stderr, "ERROR: No queda tiempo suficiente para ejecutar la operacion\n");
        return FALLO;
    }

    //validaciones del muelle
    muelle=(tMuelle*)PtrPosLista(muelles, nro_muelle-1);
    if(muelle==NULL){
        fprintf(stderr, "El Muelle M%d no existe",nro_muelle);
        return FALLO;
    }
    if(muelle->estado!=0){
        fprintf(stderr,"ERROR: El Muelle M%d no tiene ningun buque atracado", nro_muelle);
        return FALLO;
    }
    if(!ViewFirst(&(muelle->asignado.contenedores), codcont, sizeof(codcont))){
        fprintf(stderr, "ERROR: El buque asignado al muelle M%d esta vacío", nro_muelle);
        return FALLO;
    }
    zona=(tZona*)PtrPosLista(zonas, nro_zona-1);
    if(!zona){
        fprintf(stderr,"ERROR: La zona Z%d no existe", nro_zona);
        return FALLO;
    }
    if(zona->cantcont>=zona->maxcont){
        fprintf(stderr,"ERROR: La zona Z%d no tiene capacidad disponible", nro_zona);
        return FALLO;
    }
    //primero apilamos: si falla la memoria no se modifico nada
    if(!PushStack(&(zona->pilacont), codcont, strlen(codcont)+1)){
        fprintf(stderr, "ERROR: memoria insuficiente para almacenar el contenedor");
        return ERROR_MALLOC;
    }
    DeQueue(&(muelle->asignado.contenedores), codcont, sizeof(codcont));
    zona->cantcont++;

    printf("\nSe descargo el contenedor %s desde el muelle [M%d] a la zona de almacenamiento [Z%d]\n", codcont, muelle->nro, zona->nrozona);
    fprintf(log, "CODIGO:DES\tTiempo de Inicio:%02d\t Tiempo de Fin:%02d\tContenedor:%s,\tOrigen:M%d\tDestino:Z%d\n", *tiempo, *tiempo+conf.tiempo_descarga, codcont,muelle->nro, zona->nrozona);
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

int Reubicar(tLista *zonas, int nro_origen, int nro_destino, tOperador* usuario, FILE* log, int *tiempo, tConfig conf){
    char codcont[TAM_COD];
    tZona *zona_ori, *zona_dest;

    if(*tiempo+conf.tiempo_reubicacion > conf.duracion_jornada)
    {
        fprintf (stderr, "ERROR: No queda tiempo suficiente para ejecutar la operacion\n");
        return FALLO;
    }

    if(nro_origen==nro_destino){
        fprintf(stderr,"ERROR: La zona origen y destino deben ser distintas\n");
        return FALLO;
    }
    zona_ori=(tZona*)PtrPosLista(zonas, nro_origen-1);
    zona_dest=(tZona*)PtrPosLista(zonas, nro_destino-1);
    if(!zona_ori || !zona_dest){
        fprintf(stderr, "ERROR: La zona Z%d no existe", (!zona_ori)?nro_origen:nro_destino);
        return FALLO;
    }
    if(zona_ori->cantcont==0){
        fprintf(stderr, "ERROR: La zona Z%d se encuentra vacia", nro_origen);
        return FALLO;
    }
    if(zona_dest->cantcont==zona_dest->maxcont){
        fprintf(stderr,"ERROR: La zona Z%d no tiene capacidad disponible", nro_destino);
        return FALLO;
    }
    PeekStack(&(zona_ori->pilacont), codcont, sizeof(codcont));
    if(!PushStack(&(zona_dest->pilacont), codcont, strlen(codcont)+1)){
        fprintf(stderr, "ERROR: Memoria Insuficiente para reubicar el contenedor");
        return ERROR_MALLOC;
    }
    PopStack(&(zona_ori->pilacont), codcont, sizeof(codcont));
    zona_ori->cantcont--;
    zona_dest->cantcont++;
    usuario->cantReubicar++;

    fprintf(log, "CODIGO:REU\tTiempo de Inicio:%02d\tTiempo de Fin:%02d\tContenedor:%s\tOrigen:Z%d\tDestino:Z%d\n",*tiempo, *tiempo+conf.tiempo_reubicacion, codcont, zona_ori->nrozona, zona_dest->nrozona);
    printf("\nSe reubico el contenedor %s desde la zona [Z%d] a la zona [Z%d]\n", codcont, zona_ori->nrozona, zona_dest->nrozona);
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

int Entregar(tLista *zonas, tCola *camiones, tOperador *usuario, FILE* log, int *tiempo, tConfig conf){
    int pos_zona;
    char codcont[TAM_COD];
    tCamion camion;
    tZona *zona;

    //el tiempo lo avanza quien invoca la operacion (ProcesarInstruccion) una vez que retorna EXITO
    if(*tiempo+conf.tiempo_carga > conf.duracion_jornada)
    {
        fprintf(stderr, "ERROR: No queda tiempo suficiente para ejecutar la operacion\n");
        return FALLO;
    }
    if(!ViewFirst(camiones, &camion, sizeof(tCamion)))
    {
        fprintf(stderr, "ERROR: No hay camiones esperando atencion\n");
        return FALLO;
    }
    pos_zona=BuscLista(zonas, camion.codcont, CmpContTope);
    if(pos_zona==POS_INVALIDA)
    {
        fprintf(stderr, "ERROR: El contenedor %s solicitado por %s no esta en el tope de ninguna zona\n", camion.codcont, camion.codcamion);
        return FALLO;
    }
    zona=(tZona*)PtrPosLista(zonas, pos_zona);
    PopStack(&(zona->pilacont), codcont, sizeof(codcont));
    zona->cantcont--;
    DeQueue(camiones, &camion, sizeof(tCamion));
    usuario->puntuacion += 10;
    usuario->contenEntregados++;

    printf("\nContenedor %s entregado al camion %s desde la zona [Z%d]\n", codcont, camion.codcamion, zona->nrozona);
    fprintf(log, "CODIGO:ENT\tTiempo de Inicio:%02d\tTiempo de Fin:%02d\tContenedor:%s\tOrigen:Z%d\tDestino:%s\n", *tiempo, *tiempo+conf.tiempo_carga, codcont, zona->nrozona, camion.codcamion);
    return EXITO;
}

void opsDisponibles ()
{
    puts ("OPERACIONES DISPONIBLES:");
    puts ("DES - Descargar y almacenar. Sintaxis: DES <muelle> <zona>");
    puts ("REU - Reubicar. Sintaxis: REU <zona_origen> <zona_destino>");
    puts ("ENT - Entregar contenedor al proximo camion. Sintaxis: ENT");
    puts ("VER - Ver estado. Sintaxis: VER");
    puts ("ESP - Esperar un minuto. Sintaxis: ESP");
    puts ("AVZ - Esperar n minutos. Sintaxis: AVZ <minutos>");
}

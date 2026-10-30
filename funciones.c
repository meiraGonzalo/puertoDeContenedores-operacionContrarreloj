#include "funciones.h"

int BuscarParametro(char *parametro,char *param_validos[]){
    int i=0;
    while(param_validos[i]!=NULL){
        if(strcmpi(param_validos[i], parametro)==0)
            return i;
        i++;
    }
    return POS_INVALIDA;
}


int GenerarSimulacion(char *sim, tConfig *conf, tCola *buques, tCola *camiones){
    tBuque buque;
    tCamion camion;
    char codcont[TAM_COD];
    int i, j, total_contenedores=0, cant_contenedores, cant_camiones, posrandom;
    int presupuesto=conf->duracion_jornada*65/100; //maximo de minutos de trabajo obligatorio (descargar + entregar)
    int tope_buques=presupuesto/(conf->tiempo_descarga+conf->tiempo_carga); //como minimo cada buque trae un contenedor
    int cant_buques;
    int max_por_buque;
    int ultimo_camion=conf->duracion_jornada-conf->tiempo_carga; //un camion que llega despues ya no se puede atender
    tLista lista;
    FILE *fpuerto;

    if(tope_buques>conf->cant_max_buques) tope_buques=conf->cant_max_buques;
    if(tope_buques<1) tope_buques=1;
    cant_buques=GenerarRandom((tope_buques+1)/2, tope_buques);
    max_por_buque=presupuesto/(cant_buques*(conf->tiempo_descarga+conf->tiempo_carga)); //cada contenedor cuesta como maximo descarga + carga
    if(max_por_buque>conf->cant_max_contenedores) max_por_buque=conf->cant_max_contenedores;
    if(max_por_buque<1) max_por_buque=1;
    if(ultimo_camion<0) ultimo_camion=0;
    int tiemposBuques[cant_buques];

    fpuerto=fopen(sim, "wt");
    if(!fpuerto)
        return ERROR_ARCHIVO;
    CrearLista(&lista);
    fprintf(fpuerto,"JORNADA: %d\n", conf->duracion_jornada);
    fprintf(fpuerto,"MUELLES: %d\n", conf->cantidad_muelles);
    fprintf(fpuerto,"ZONAS: %d\n",conf->cantidad_zonas);
    fprintf(fpuerto,"CAPACIDAD_PILA: %d\n",conf->capacidad_pila);

    //los buques llegan en la primera mitad de la jornada y el primero en T=0
    fprintf(fpuerto,"\n[BUQUES]\n");
    for(i=0;i<cant_buques;i++)
        tiemposBuques[i]=GenerarRandom(0, conf->duracion_jornada/2);
    ordenarTiempos(tiemposBuques, cant_buques);
    tiemposBuques[0]=0;

    for(i=0;i<cant_buques;i++){
        cant_contenedores=GenerarRandom((max_por_buque+1)/2, max_por_buque);
        sprintf(buque.cod, "B%03d", i+1);
        buque.tiempo=tiemposBuques[i];
        CreateQueue(&buque.contenedores);
        fprintf(fpuerto,"%s;T=%d;C=", buque.cod, buque.tiempo);
        for(j=0;j<cant_contenedores;j++){
            sprintf(codcont,"C%d%02d", i+1, j+1);
            fprintf(fpuerto,(j<cant_contenedores-1)?"%s,":"%s", codcont);
            if(!EnQueue(&buque.contenedores, codcont, strlen(codcont)+1) || !InsPrinLista(&lista, codcont, strlen(codcont)+1)){
                fclose(fpuerto);
                return ERROR_MALLOC;
            }
            total_contenedores++;
        }
        fprintf(fpuerto,"\n");
        if(!EnQueue(buques, &buque, sizeof(tBuque))){
            fclose(fpuerto);
            return ERROR_MALLOC;
        }
    }

    //entre el 60% y el 100% de los contenedores son pedidos por un camion (sin superar el maximo configurado)
    cant_camiones=GenerarRandom((total_contenedores*60+99)/100, total_contenedores);
    if(cant_camiones>conf->cant_max_camiones) cant_camiones=conf->cant_max_camiones;
    int tiemposCamiones[cant_camiones];
    for(i=0;i<cant_camiones;i++)
        tiemposCamiones[i]=GenerarRandom(0, ultimo_camion);
    ordenarTiempos(tiemposCamiones, cant_camiones);

    fprintf(fpuerto,"\n[CAMIONES]\n");
    for(i=0;i<cant_camiones;i++){
        posrandom=GenerarRandom(0, total_contenedores-1);
        OutPosLista(&lista, codcont, sizeof(codcont), posrandom); //lo saca de la lista: un contenedor no lo piden dos camiones
        total_contenedores--;
        sprintf(camion.codcamion, "K%03d", i+1);
        strcpy(camion.codcont, codcont);
        camion.tiempo=tiemposCamiones[i];
        fprintf(fpuerto,"%s;T=%d;C=%s\n", camion.codcamion, camion.tiempo, camion.codcont);
        if(!EnQueue(camiones, &camion, sizeof(tCamion))){
            fclose(fpuerto);
            return ERROR_MALLOC;
        }
    }
    fclose(fpuerto);
    VaciarLista(&lista);
    return EXITO;
}

int GenerarRandom(int min, int max){
    if(max<min)
        return min;
    return min+ rand()%(max-min+1);
}

int ValidarRangoInt(int num, int liminf, int limsup){
    return (num>=liminf && num<=limsup)?1:0;
}

int InicializarZonas(tLista *zonas, int cant_zonas, int cap_pila){
    tZona zona;
    while(cant_zonas){
        zona.nrozona=cant_zonas;
        zona.cantcont=0;
        zona.maxcont=cap_pila;
        CreateStack(&(zona.pilacont));
        if(!InsPrinLista(zonas, &zona, sizeof(tZona)))
        {
            VaciarLista(zonas);
            return ERROR_MALLOC;
        }
        cant_zonas--;
    }
    return EXITO;
}

int ProcesarInstruccion(char *linea,tLista *muelles, tLista *zonas, tCola* buquesEspera, tCola *camiones, int *tiempo, tConfig conf, tOperador* usuario, FILE* log, int* salida){
    int pos_instr, param1, param2 ;
    char *aux;
    char *instr_validas[]={"DES","REU","VER","ENT","ESP", "OPS", "EXT", NULL};
    char c1, c2;
    char cmd[TAM_CONSOLA]="", p1[TAM_CONSOLA]="", p2[TAM_CONSOLA]="", sobra[TAM_CONSOLA]="", normalizada[4*TAM_CONSOLA];
    int cant_tokens=sscanf(linea, "%s %s %s %s", cmd, p1, p2, sobra);
    if(cant_tokens<1)
        return INSTR_INVALIDA;
    sprintf(normalizada, "%s %s %s\n", cmd, p1, p2); //una sola separacion entre palabras
    linea=normalizada;
    aux=strchr(linea,' ');
    if(!aux)
        aux=strchr(linea,'\n');
    if(aux) {
        *aux = 0;
    }
    pos_instr=BuscarParametro(linea, instr_validas);
    if(pos_instr==-1)
        return INSTR_INVALIDA;
    if (strlen(linea) == 0) {
        return INSTR_INVALIDA;
    }
    if((pos_instr<=1 && cant_tokens!=3) || (pos_instr>1 && cant_tokens!=1)) //DES y REU llevan 2 parametros; el resto ninguno
        return PARAM_INVALIDO;
    if(pos_instr==0 || pos_instr==1){//buscamos los parametro si es DES o REU
        if(!aux)
            return PARAM_INVALIDO;
        linea=aux+1;
        aux=strchr(linea, ' ');
        if(!aux)
            return PARAM_INVALIDO;
        *aux = 0;
        if (sscanf(linea, "%c%d", &c1, &param1) != 2) {
            return PARAM_INVALIDO;
        }
        c1 = toupper(c1);
        linea=aux+1;
        aux=strchr(linea, '\n');
        if(aux) {
            *aux = 0;
        }
        if (sscanf(linea, "%c%d", &c2, &param2) != 2) {
            return PARAM_INVALIDO;
        }
        c2 = toupper(c2);
        if(pos_instr==0 && (c1!='M' || c2!='Z' || !ValidarRangoInt(param1,1,conf.cantidad_muelles) || !ValidarRangoInt(param2,1,conf.cantidad_zonas)))
            return PARAM_INVALIDO;
        if(pos_instr==1 && (c1!='Z' || c2!='Z' || !ValidarRangoInt(param1,1,conf.cantidad_zonas) || !ValidarRangoInt(param2,1,conf.cantidad_zonas)))
            return PARAM_INVALIDO;
    }
    switch(pos_instr){
        case 0: //DES
            if(!Descargar(muelles, zonas, param1, param2, log, tiempo, conf))
            {
                return FALLO;
            }
            else *tiempo+=conf.tiempo_descarga;
            break;
        case 1: //REU
            if(!Reubicar(zonas, param1, param2, usuario, log, tiempo, conf)){
                return FALLO;
            }
            else{
                *tiempo+=conf.tiempo_reubicacion;
            }
            break;
        case 2: //VER
            system("cls");
            printf ("TIEMPO ACTUAL DE LA JORNADA: %d\n", *tiempo);
            printf ("\nPUNTOS ACUMULADOS: %d\n\n", usuario->puntuacion);
            printf ("MUELLES OCUPADOS:\n");
            RecorrerLista(muelles, MostrarMuelle, NULL);
            puts("");
            RecorrerLista(zonas, VerZona, NULL);
            puts("");
            puts("");
            VerCamiones(camiones, VER_CAMIONES,*tiempo);
            printf ("Hay [%d] Buques en espera\n\n", contarBuquesEspera(buquesEspera));
            break;
        case 3: //ENT
            if(!Entregar(zonas,camiones, usuario, log, tiempo, conf)){
                return FALLO;
            }
            else{
            *tiempo+=conf.tiempo_carga;
            }
            break;
        case 4://ESP
            if(*tiempo+1 > conf.duracion_jornada)
            {
                fprintf (stderr, "ERROR: No queda tiempo suficiente para ejecutar la operacion\n");
                return FALLO;
            }
            fprintf(log, "CODIGO:ESP\tTiempo de Inicio:%02d\tTiempo de Fin:%02d\n", *tiempo, *tiempo+1);
            *tiempo+=1;
            printf("Se avanzo la jornada 1 minuto..\n\n");
            break;
        case 5://OPS
            opsDisponibles();
            break;
        case 6://EXT
            salidaAnticipada(log, salida, tiempo);
            break;
        default:
            printf ("Operacion no reconocida\n");
            break;
    }
    return EXITO;
}

void muellesVacios (void * a, void* flag)
{
    tMuelle* m = (tMuelle*)a;
    int* f = (int*)flag;
    if (m->estado == 0) *f = m->estado;
}

int EscenarioRealizable (tConfig conf)
{
    int tiempoOperacion = conf.cant_max_buques*conf.cant_max_contenedores*conf.tiempo_descarga + conf.cant_max_camiones*conf.tiempo_carga;

    if (tiempoOperacion > conf.duracion_jornada)
    {
        fprintf (stderr, "WARNING: El tiempo de operacion del puerto en su maxima capacida excede la duracion de la jornada\n");
        return FALLO;
    }
    if (conf.cantidad_muelles <=0 || conf.cantidad_zonas <=0 || conf.capacidad_pila <= 0)
    {
        fprintf(stderr, "WARNING: El puerto carece de infraestructura suficiente (muelles, zonas y pilas) para procesar las operaciones\n");
        return FALLO;
    }

    return EXITO;
}

void ordenarTiempos(int tiemposGenerados[], int cantidadElementos)
{
    int i, j, temp;

    for (i = 0; i < cantidadElementos - 1; i++)
    {
        for (j = 0; j < cantidadElementos - i - 1; j++)
        {
            if (tiemposGenerados[j] > tiemposGenerados[j + 1])
            {
                temp = tiemposGenerados[j];
                tiemposGenerados[j] = tiemposGenerados[j + 1];
                tiemposGenerados[j + 1] = temp;
            }
        }
    }
}

int contarBuquesEspera (tCola* buquesEspera)
{
    int i= 0;
    tBuque b;
    tCola aux;
    CreateQueue(&aux);
    while(DeQueue(buquesEspera, &b, sizeof(tBuque)))
    {
        i++;
        EnQueue(&aux, &b, sizeof(tBuque));
    }
    while(DeQueue(&aux, &b, sizeof(tBuque)))
        EnQueue(buquesEspera, &b, sizeof(tBuque));

    return i;
}

int todosMuellesVacios(tLista* muelles)
{
    int todosVacios = 1;
    RecorrerLista(muelles, muellesVacios, &todosVacios);
    return todosVacios;
}

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

int CargarConfiguracion(char *nomarch, tConfig *conf){
    int valor,pos_param;
    FILE *fp=fopen(nomarch, "rt");
    char buffer[MAX_COUNT], *aux, *parametro, *linea;
    char *param_validos[]={"duracion_jornada_minutos","cantidad_muelles","cantidad_zonas_almacenamiento","capacidad_pila","maximo_buques","maximo_camiones",
    "maximo_contenedores_por_buque","tiempo_descarga_contenedor","tiempo_reubicacion_contenedor","tiempo_carga_camion",NULL};
    if(!fp)
        return ERROR_ARCHIVO;
    while(fgets(buffer, MAX_COUNT, fp)){
        linea=buffer;
        while(*linea==' ')
            linea++;
        aux=strchr(linea,':');
        *aux=0;
        parametro=linea;
        pos_param=BuscarParametro(parametro, param_validos);
        linea=aux+1;
        aux=strchr(linea,'\n');
        if(aux)
            *aux=0;
        sscanf(linea, "%d", &valor);
        if(valor<0){
            fclose(fp);
            return CONF_INVALIDA;
        }
        switch(pos_param){
            case 0:
                conf->duracion_jornada=valor;
                break;
            case 1:
                conf->cantidad_muelles=valor;
                break;
            case 2:
                conf->cant_zonas=valor;
                break;
            case 3:
                conf->cap_pila=valor;
            case 4:
                conf->max_buques=valor;
                break;
            case 5:
                conf->max_camiones=valor;
                break;
            case 6:
                conf->max_cont_buque=valor;
                break;
            case 7:
                conf->tiempo_descarga=valor;
                break;
            case 8:
                conf->tiempo_reubi=valor;
                break;
            case 9:
                conf->tiempo_carga=valor;
                break;
            default:
        }
    }
    fclose(fp);
    return EXITO;
}

int GenerarSimulacion(char *sim, tConfig *conf, tCola *buques, tCola *camiones){
    tBuque buque;
    tCamion camion;
    char codcont[TAM_COD];
    int i,j,total_contenedores=0,cant_buques, cant_camiones,cant_contenedores, posrandom,tiempo=0;
    tLista lista;
    FILE *fpuerto=fopen(sim, "wt");
    if(!fpuerto){
        return ERROR_ARCHIVO;
    }
    CrearLista(&lista);
    fprintf(fpuerto,"JORNADA: %d\n", conf->duracion_jornada);
    fprintf(fpuerto,"MUELLES: %d\n", conf->cantidad_muelles);
    fprintf(fpuerto,"ZONAS: %d\n",conf->cant_zonas);
    fprintf(fpuerto,"CAPACIDAD_PILA: %d\n",conf->cap_pila);
    fprintf(fpuerto,"\n[BUQUES]\n");
    cant_buques=GenerarRandom(MIN_BUQUES, conf->max_buques);
    i=0;
    while(i<cant_buques){
        if(i!=0) //el primero de los buques siempre llegara en t=0
            tiempo=GenerarRandom(tiempo, conf->duracion_jornada -FIN_LLEGADA); //asi se mantiene el orden de llegada en el archivo
        fprintf(fpuerto,"B00%d;T=%d;C=",i+1,tiempo);
        cant_contenedores=GenerarRandom(MIN_CONT_BUQUE,conf->max_cont_buque);
        j=0;
        sprintf(buque.cod, "B00%d",i+1);
        buque.tiempo=tiempo;
        CreateQueue(&buque.contenedores);
        while(j<cant_contenedores){
            fprintf(fpuerto,"C%d0%d",i+1,j+1);
            if(j<cant_contenedores-1)
                fprintf(fpuerto,",");
            sprintf(codcont,"C%d0%d", i+1,j+1);
            EnQueue(&buque.contenedores, codcont, strlen(codcont)+1);
            InsPrinLista(&lista,codcont, strlen(codcont)+1);
            total_contenedores++;
            j++;
        }
        EnQueue(buques, &buque, sizeof(tBuque));
        fprintf(fpuerto, "\n");
        i++;
    }
    fprintf(fpuerto,"\n[CAMIONES]\n");
    i=0;
    tiempo=0;
    cant_camiones=GenerarRandom(MIN_CAMIONES, conf->max_camiones);
    cant_camiones=(cant_camiones>total_contenedores)?total_contenedores:cant_camiones;
    while(i<cant_camiones){
        posrandom=GenerarRandom(0, total_contenedores-1);
        if(i!=0)
            tiempo=GenerarRandom(tiempo, conf->duracion_jornada -FIN_LLEGADA);
        OutPosLista(&lista, codcont, sizeof(codcont),posrandom);
        fprintf(fpuerto,"K00%d;T=%d;C=%s\n",i+1,tiempo,codcont);
        sprintf(camion.codcamion,"K00%d",i+1);
        strcpy(camion.codcont, codcont);
        camion.tiempo=tiempo;
        EnQueue(camiones, &camion, sizeof(tCamion));
        total_contenedores--;
        i++;
    }
    fclose(fpuerto);
    VaciarLista(&lista);
    return EXITO;
}

int GenerarRandom(int min, int max){
    max=(min+3>max)?max:min+3;
    return min+ rand() % (max-min+1);
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
            return ERROR_MALLOC;
        cant_zonas--;
    }
    return EXITO;
}

int ProcesarInstruccion(char *linea,tLista *muelles, tLista *zonas, tCola *camiones, int *tiempo, tConfig conf){
    int pos_instr, param1, param2 ;
    char *aux;
    char *instr_validas[]={"DES","REU","VER","ENT","ESP"};
    aux=strchr(linea,' ');
    if(!aux)
        aux=strchr(linea,'\n');
    *aux=0;
    pos_instr=BuscarParametro(linea, instr_validas);
    if(pos_instr==-1)
        return INSTR_INVALIDA;
    if(pos_instr==0 || pos_instr==1){//buscamos los parametro si es DES o REU
        linea=aux+1;
        aux=strchr(linea, ' ');
        if(!aux)
            return PARAM_INVALIDO;
        *aux=0;
        sscanf(linea, "%d", &param1);
        linea=aux+1;
        aux=strchr(linea, '\n');
        *aux=0;
        sscanf(linea, "%d", &param2);
        if(pos_instr==0 && (!ValidarRangoInt(param1,1,conf.cantidad_muelles) || !ValidarRangoInt(param2, 1,conf.cant_zonas)))
            return PARAM_INVALIDO;
        if(pos_instr==1 && (!ValidarRangoInt(param1,1,conf.cant_zonas) || !ValidarRangoInt(param2, 1,conf.cant_zonas)))
            return PARAM_INVALIDO;
    }
    switch(pos_instr){
        case 0: //DES
            Descargar(muelles, zonas, param1, param2);
            *tiempo+=conf.tiempo_descarga;
            printf("T=%d\n",*tiempo);
            RecorrerLista(zonas, VerZona, NULL);
            printf("\n");
            break;
        case 1: //REU
            if(!Reubicar(zonas, param1, param2))
                fprintf(stderr,"ERROR: Verificar que la zona origen no este vacia y que la zona destino no este llena\n");
            else{
                *tiempo+=conf.tiempo_reubi;
                printf("T=%d\n",*tiempo);
            }

            RecorrerLista(zonas, VerZona, NULL);
            printf("\n");
            break;
        case 2: //VER
            VerCamiones(camiones, VER_CAMIONES,*tiempo);
            break;
        case 3: //ENT
            if(!Entregar(zonas,camiones))
                fprintf(stderr, "ERROR: El contenedor pedido no esta en el tope de la pila de una zona\n");
            else{
            *tiempo+=conf.tiempo_carga;
            printf("T=%d\n", *tiempo);
            }
            break;
        case 4://ESP
            *tiempo+=1;
            printf("T=%d\n", *tiempo);

            break;
    }
    return EXITO;
}

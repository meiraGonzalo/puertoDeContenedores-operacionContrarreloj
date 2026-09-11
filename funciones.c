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

int GenerarSimulacion(char *sim, tConfig *conf){
    char codcont[30];
    int i,j,total_contenedores=0,cant_buques, cant_camiones,cant_contenedores, posrandom;
    tLista lista;
    CrearLista(&lista);
    FILE *fpuerto=fopen(sim, "wt");
    if(!fpuerto){
        return ERROR_ARCHIVO;
    }
    fprintf(fpuerto,"JORNADA: %d\n", conf->duracion_jornada);
    fprintf(fpuerto,"MUELLES: %d\n", conf->cantidad_muelles);
    fprintf(fpuerto,"ZONAS: %d\n",conf->cant_zonas);
    fprintf(fpuerto,"CAPACIDAD_PILA: %d\n",conf->cap_pila);
    fprintf(fpuerto,"\n[BUQUES]\n");
    cant_buques=GenerarRandom(1, conf->max_buques);
    i=0;
    while(i<cant_buques){
        fprintf(fpuerto,"B00%d;T=%d;C=",i+1,GenerarRandom(0,conf->duracion_jornada -FIN_LLEGADA));
        cant_contenedores=GenerarRandom(1,conf->max_cont_buque);
        j=0;
        while(j<cant_contenedores){
            fprintf(fpuerto,"C%d0%d",i+1,j+1);
            if(j<cant_contenedores-1)
                fprintf(fpuerto,",");
            sprintf(codcont,"C%d0%d", i+1,j+1);
            InsPrinLista(&lista,codcont, strlen(codcont)+1);
            total_contenedores++;
            j++;
        }
        fprintf(fpuerto, "\n");
        i++;
    }
    fprintf(fpuerto,"\n[CAMIONES]\n");
    i=0;
    cant_camiones=GenerarRandom(1, conf->max_camiones);
    cant_camiones=(cant_camiones>total_contenedores)?total_contenedores:cant_camiones;
    while(i<cant_camiones){
        posrandom=GenerarRandom(0, total_contenedores-1);
        OutPosLista(&lista, codcont, sizeof(codcont),posrandom);
        fprintf(fpuerto,"K00%d;T=%d;C=%s\n",i+1,GenerarRandom(0,conf->duracion_jornada-FIN_LLEGADA),codcont);
        total_contenedores--;
        i++;
    }
    fclose(fpuerto);
    VaciarLista(&lista);
    return EXITO;
}

int GenerarRandom(int min, int max){
    return min+ rand() % (max-min+1);
}



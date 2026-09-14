#include "funciones.h"
#include "asignacion_buques/buque_muelle.h"
#include "operaciones/operaciones.h"

#define ERROR 1
#define FIN 0

int main()
{
    char buffer_consola[TAM_CONSOLA];
    int tiempo=0;
    srand(time(NULL));
    tConfig conf;
    tCola buques;
    tCola camiones;
    tLista muelles;
    tLista zonas;
    CreateQueue(&buques);
    CreateQueue(&camiones);
    CrearLista(&muelles);
    CrearLista(&zonas);
    if(!CargarConfiguracion("config.txt", &conf)){
        fprintf(stderr,"ERROR: carga de archivo config.txt no realizada");
        return ERROR;
    }
    if(!GenerarSimulacion("puerto.txt",&conf,&buques, &camiones)){
        fprintf(stderr,"ERROR: generacion de archivo puerto.txt no realizada");
        return ERROR;
    }
    InicializarMuelles(&muelles, conf.cantidad_muelles);
    InicializarZonas(&zonas, conf.cant_zonas, conf.cap_pila);

    while(tiempo<=conf.duracion_jornada){
        EmbarcarBuquesVacios(&muelles);
        AsignarBuques(&buques, &muelles, tiempo);
        RecorrerLista(&muelles, MostrarMuelle, NULL);
        printf("\n<Operador>");
        fgets(buffer_consola, sizeof(buffer_consola), stdin);
        ProcesarInstruccion(buffer_consola, &muelles, &zonas, &camiones, &tiempo, conf);
    }
    VerCamiones(&camiones, 3, tiempo);
    VerCamiones(&camiones,4, tiempo);
    return FIN;
}

#include "funciones.h"
#include "asignacion_buques/buque_muelle.h"

#define ERROR 1
#define FIN 0

int main()
{
    srand(time(NULL));
    tConfig conf;
    tCola buques;
    tCola camiones;
    tLista muelles;
    CreateQueue(&buques);
    CreateQueue(&camiones);
    CrearLista(&muelles);
    if(!CargarConfiguracion("config.txt", &conf)){
        fprintf(stderr,"ERROR: carga de archivo config.txt no realizada");
        return ERROR;
    }
    if(!GenerarSimulacion("puerto.txt",&conf,&buques, &camiones)){
        fprintf(stderr,"ERROR: generacion de archivo puerto.txt no realizada");
        return ERROR;
    }
    InicializarMuelles(&muelles, conf.cantidad_muelles);
    AsignarBuques(&buques, &muelles, 20);
    RecorrerLista(&muelles, MostrarMuelle, NULL);
    return FIN;
}

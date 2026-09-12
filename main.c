#include "funciones.h"
#include "asignacion_buques/buque_muelle.h"
#include "operaciones/operaciones.h"

#define ERROR 1
#define FIN 0

int main()
{
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
    AsignarBuques(&buques, &muelles, 20);
    RecorrerLista(&muelles, MostrarMuelle, NULL);
    InicializarZonas(&zonas, conf.cant_zonas, conf.cap_pila);
    VerCamiones(&camiones, 3);
    VerCamiones(&camiones,4);
    return FIN;
}

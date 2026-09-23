#include "simulacion.h"

int simulacion()
{
    tConfig config;

    system("cls");
    if(!cargarConfiguracion(&config))
    {
        system("pause"); //pausa para ver el mensaje de error
        return FALLO;
    }
    char buffer_consola[TAM_CONSOLA];
    int tiempo=0;
    srand(time(NULL));
    tCola buques;
    tCola camiones;
    tLista muelles;
    tLista zonas;
    CreateQueue(&buques);
    CreateQueue(&camiones);
    CrearLista(&muelles);
    CrearLista(&zonas);

    if(!GenerarSimulacion("puerto.txt",&config,&buques, &camiones)){
        fprintf(stderr,"ERROR: generacion de archivo puerto.txt no realizada");
        return FALLO;
    }
    InicializarMuelles(&muelles, config.cantidad_muelles);
    InicializarZonas(&zonas, config.cantidad_zonas, config.capacidad_pila);

    while(tiempo<=config.duracion_jornada){
        EmbarcarBuquesVacios(&muelles);
        AsignarBuques(&buques, &muelles, tiempo);
        RecorrerLista(&muelles, MostrarMuelle, NULL);
        printf("\n<Operador>");
        fgets(buffer_consola, sizeof(buffer_consola), stdin);
        ProcesarInstruccion(buffer_consola, &muelles, &zonas, &camiones, &tiempo, config);
    }
    return 0;
}

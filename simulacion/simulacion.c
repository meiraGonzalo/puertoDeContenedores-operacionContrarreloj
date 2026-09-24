#include "simulacion.h"

int simulacion()
{
    tConfig config;
    tEstado sim;
    tCola buquesEspera;
    tCola camionesEspera;

    system("cls");
    if(!cargarConfiguracion(&config))
    {
        system("pause"); //pausa para ver el mensaje de error
        return FALLO;
    }

    char buffer_consola[TAM_CONSOLA];
    srand(time(NULL));

    estadoIniciar(&sim);
    CreateQueue(&buquesEspera);
    CreateQueue(&camionesEspera);

    if(!GenerarSimulacion("puerto.txt",&config,&sim.buques_programados, &sim.camiones_programados)){
        fprintf(stderr,"ERROR: generacion de archivo puerto.txt no realizada");
        return FALLO;
    }
    if(!InicializarMuelles(&sim.muelles, config.cantidad_muelles))
    {
        fprintf(stderr,"ERROR: memoria insuficiente para crear lista de muelles\n");
        return FALLO;
    }
    if(!InicializarZonas(&sim.zonas, config.cantidad_zonas, config.capacidad_pila))
    {
        fprintf(stderr,"ERROR: memoria insuficiente para crear lista de zonas de almacenamiento\n");
        return FALLO;
    }

    while(sim.tiempo_actual<=config.duracion_jornada && !bloqueoOperativo(&sim, &camionesEspera)){
        EmbarcarBuquesVacios(&sim.muelles);
        AsignarBuques(&sim.buques_programados, &sim.muelles, sim.tiempo_actual);
        RecorrerLista(&sim.muelles, MostrarMuelle, NULL);
        printf("\n<Operador>");
        fgets(buffer_consola, sizeof(buffer_consola), stdin);
        ProcesarInstruccion(buffer_consola, &sim.muelles, &sim.zonas, &camionesEspera, &sim.tiempo_actual, config);
    }
    return EXITO;
}

void estadoIniciar(tEstado* sistema)
{
    CreateQueue(&sistema->buques_programados);
    CreateQueue(&sistema->camiones_programados);
    CrearLista(&sistema->muelles);
    CrearLista(&sistema->zonas);
    sistema->tiempo_actual = 0;
}

int bloqueoOperativo (tEstado* sis, tCola* camionesEspera)
{
    int bloqueo = EXITO, flag = 0;

    if (!IsEmptyQueue(&sis->buques_programados))
    {
        bloqueo = FALLO;
    }
    if(!IsEmptyQueue(&sis->camiones_programados))
    {
        bloqueo = FALLO;
    }
    if(!IsEmptyQueue(camionesEspera))
    {
        bloqueo = FALLO;
    }
    RecorrerLista(&sis->muelles, muellesVacios, &flag);
    if (flag)
    {
        bloqueo = FALLO;
    }
    return bloqueo;
}

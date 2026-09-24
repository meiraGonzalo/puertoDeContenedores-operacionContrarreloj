#include "simulacion.h"

int simulacion()
{
    tConfig config;
    tEstado sim;
    tOperador usuario;
    tCola buquesEspera;
    tCola camionesEspera;
    tCamion c;
    int camionesPendientes = 0;
    FILE* log;

    system("cls");
    if(!cargarConfiguracion(&config))
    {
        system("pause"); //pausa para ver el mensaje de error
        return FALLO;
    }

    char buffer_consola[TAM_CONSOLA];
    srand(time(NULL));

    estadoIniciar(&sim);
    iniciarUsuario(&usuario);
    CreateQueue(&buquesEspera);
    CreateQueue(&camionesEspera);
    if (!EscenarioRealizable(config))
    {
        system ("pause");
        //return FALLO; por ahora se permite que la simulacion pueda no ser correctamente finalizable
    }

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

    log = fopen("log.txt", "w+t");
    if (!log)
    {
        fprintf (stderr, "ERROR: no se pudo generar el archivo de log temporales\n");
        return FALLO;
    }
    system("pause");
    system("cls");
    printf("Bievenido a Operacion Contrarreloj!\n\n");
    while(sim.tiempo_actual<config.duracion_jornada && !bloqueoOperativo(&sim, &camionesEspera)){
        if(IsEmptyQueue(&camionesEspera) && todosMuellesVacios(&sim.muelles))
            avanzarRelojAuto(&sim);
        EmbarcarBuquesVacios(&sim.muelles, &usuario);
        AsignarBuques(&sim.buques_programados, &sim.muelles, &buquesEspera, sim.tiempo_actual);
        asignarCamiones(&sim, &camionesEspera);
        printf("<Operador>");
        fgets(buffer_consola, sizeof(buffer_consola), stdin);
        ProcesarInstruccion(buffer_consola, &sim.muelles, &sim.zonas, &buquesEspera, &camionesEspera, &sim.tiempo_actual, config, &usuario, log);
    }
    system ("cls");
    if(bloqueoOperativo(&sim, &camionesEspera))
        printf ("Finaliza la jornada por no quedar acciones significativas por realizarse\n");
    else if (sim.tiempo_actual<=config.duracion_jornada)
        printf ("Finaliza la jornada por cumplirse su duracion establecida\n");
    while (DeQueue(&camionesEspera, &c, sizeof(tCamion)))
    {
        camionesPendientes++;
    }
    usuario.camionesPend = camionesPendientes;
    usuario.puntuacion -= camionesPendientes*2;
    mostrarResumenUser(&config, &sim, &usuario);
    system ("pause");

    system("cls");
    mostrarResumenMov(log);
    system("pause");

    fclose(log);
    remove("log.txt");
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
    int bloqueo = EXITO;

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
    if (!todosMuellesVacios(&sis->muelles))
    {
        bloqueo = FALLO;
    }
    return bloqueo;
}

void avanzarRelojAuto (tEstado* sim)
{
    tBuque b;
    tCamion c;
    int minutos;
    if (!ViewFirst(&sim->buques_programados, &b, sizeof(tBuque))) b.tiempo = sim->tiempo_actual;
    if (!ViewFirst(&sim->camiones_programados, &c, sizeof(tCamion))) c.tiempo = sim->tiempo_actual;
    minutos = (b.tiempo-sim->tiempo_actual) <= (c.tiempo - sim->tiempo_actual) ? (b.tiempo-sim->tiempo_actual) : (c.tiempo - sim->tiempo_actual);
    sim->tiempo_actual += minutos;
    printf ("Se avanzo automaticamente %d minuto/s\n", minutos);
}

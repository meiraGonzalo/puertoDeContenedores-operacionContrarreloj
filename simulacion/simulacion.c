#include "simulacion.h"
#include "../usuario/usuario.h"

int simulacion()
{
    tConfig config;
    tEstado sim;
    tOperador usuario;
    tCola buquesEspera;
    tCola camionesEspera;
    tCamion c;
    int camionesPendientes = 0, cantMov, salir = FALLO;
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
    puts ("Puede seleccionar el comando OPS para ver las operaciones disponibles");
    procesarEventos(&sim, &buquesEspera, &camionesEspera, &usuario); //eventos programados para T=0
    avanzarRelojAuto(&sim, &buquesEspera, &camionesEspera, &usuario, &config);
    while(!jornadaFinalizada(&sim, &buquesEspera, &camionesEspera, &config) && salir == FALLO)
    {
        printf("<Operador>");
        fgets(buffer_consola, sizeof(buffer_consola), stdin);
        ProcesarInstruccion(buffer_consola, &sim.muelles, &sim.zonas, &buquesEspera, &camionesEspera, &sim.tiempo_actual, config, &usuario, log, &salir);
        procesarEventos(&sim, &buquesEspera, &camionesEspera, &usuario);
        avanzarRelojAuto(&sim, &buquesEspera, &camionesEspera, &usuario, &config);
    }
    system ("cls");
    if (salir == EXITO)
        printf ("Finaliza la jornada de manera anticipada por decision del usuario\n");
    else if(finalizacionNormal(&sim, &buquesEspera, &camionesEspera, &config))
        printf ("Finaliza la jornada exitosamente: no quedan eventos, buques ni camiones por atender\n");
    else if (sim.tiempo_actual>=config.duracion_jornada)
        printf ("Finaliza la jornada por cumplirse su duracion establecida\n");
    else
        printf ("Finaliza la jornada por bloqueo operativo: no hay operaciones posibles para realizar\n");
    while (DeQueue(&sim.camiones_programados, &c, sizeof(tCamion)))
        EnQueue(&camionesEspera, &c, sizeof(tCamion));
    while (DeQueue(&camionesEspera, &c, sizeof(tCamion)))
    {
        camionesPendientes++;
    }
    usuario.camionesPend = camionesPendientes;
    usuario.puntuacion -= camionesPendientes*2;
    mostrarResumenUser(&config, &sim, &usuario);
    actualizarRegUsuario(&usuario);
    system ("pause");

    system("cls");
    cantMov=mostrarResumenMov(log);
    system("pause");
    archivarJornada(log, &usuario, cantMov);
    fclose(log);
    remove("log.txt");
    vaciarSimulacion(&sim, &camionesEspera, &buquesEspera);
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

/* ---------- Eventos ---------- */

void procesarEventos(tEstado* sis, tCola* buquesEspera, tCola* camionesEspera, tOperador* usuario)
{
    EmbarcarBuquesVacios(&sis->muelles, usuario);                              // 1) consecuencias de la operacion: libera muelles
    AsignarBuques(&sis->buques_programados, &sis->muelles, buquesEspera, sis->tiempo_actual); // 2) arribos de buques + asignacion a muelles
    asignarCamiones(sis, camionesEspera);                                      // 3) arribos de camiones
}

/* Minuto del proximo evento programado dentro de la jornada, o -1 si no queda ninguno */
int proximoEvento(tEstado* sis, int duracion)
{
    tBuque b;
    tCamion k;
    int prox = -1;

    if (ViewFirst(&sis->buques_programados, &b, sizeof(tBuque)) && b.tiempo <= duracion)
        prox = b.tiempo;
    if (ViewFirst(&sis->camiones_programados, &k, sizeof(tCamion)) && k.tiempo <= duracion
        && (prox == -1 || k.tiempo < prox))
        prox = k.tiempo;
    return prox;
}

/* ---------- Operaciones validas (consultas, no modifican nada) ---------- */

static int hayZonaConLugar(tLista* zonas, int nroZonaExcluida)
{
    tZona z;
    int pos = 0, hay = 0;

    while (!hay && VerPosLista(zonas, &z, sizeof(tZona), pos++) != POS_INVALIDA)
        hay = (z.nrozona != nroZonaExcluida && z.cantcont < z.maxcont);
    return hay;
}

static int hayDescargaValida(tEstado* sis, const tConfig* conf) //hya tiempo, lugar y algo para descargar
{
    tMuelle m;
    int pos = 0, hay = 0;

    if (sis->tiempo_actual + conf->tiempo_descarga <= conf->duracion_jornada
        && hayZonaConLugar(&sis->zonas, 0))
    {
        while (!hay && VerPosLista(&sis->muelles, &m, sizeof(tMuelle), pos++) != POS_INVALIDA)
            hay = (m.estado == 0 && !IsEmptyQueue(&m.asignado.contenedores));
    }
    return hay;
}

typedef struct
{
    tLista* zonas;
    char pedidos[VER_CAMIONES][TAM_COD];
    int cantPedidos;
    int hay;
} tBusquedaReu;

// Copia los codigos pedidos por los primeros camiones de la cola (hasta max).

static int ventanaPedidos(tCola* camionesEspera, char pedidos[][TAM_COD], int max)
{
    tCamion k;
    tCola aux;
    int cant = 0;

    CreateQueue(&aux);
    while (DeQueue(camionesEspera, &k, sizeof(tCamion)))
    {
        if (cant < max)
        {
            strcpy(pedidos[cant], k.codcont);
            cant++;
        }
        EnQueue(&aux, &k, sizeof(tCamion));
    }
    while (DeQueue(&aux, &k, sizeof(tCamion)))
        EnQueue(camionesEspera, &k, sizeof(tCamion));
    return cant;
}

static int esPedido(const char* cod, char pedidos[][TAM_COD], int cant)
{
    int i = 0, es = 0;

    while (!es && i < cant)
    {
        es = (strcmpi(cod, pedidos[i]) == 0);
        i++;
    }
    return es;
}

/* Accion para RecorrerLista: marca busqueda->hay si en la zona hay, debajo del tope, un contenedor pedido
   y existe otra zona con lugar para mover el tope. */
static void zonaTapaPedido(void* info, void* param)
{
    tZona* z = (tZona*)info;
    tBusquedaReu* busqueda = (tBusquedaReu*)param;
    tPila aux;
    char cod[TAM_COD];
    int tapa = 0;

    if (!busqueda->hay && z->cantcont > 1 && hayZonaConLugar(busqueda->zonas, z->nrozona))
    {
        CreateStack(&aux);
        PopStack(&z->pilacont, cod, sizeof(cod));            //el tope no esta tapado
        PushStack(&aux, cod, strlen(cod) + 1);
        while (PopStack(&z->pilacont, cod, sizeof(cod)))
        {
            tapa = tapa || esPedido(cod, busqueda->pedidos, busqueda->cantPedidos);
            PushStack(&aux, cod, strlen(cod) + 1);
        }
        while (PopStack(&aux, cod, sizeof(cod)))
            PushStack(&z->pilacont, cod, strlen(cod) + 1);
        busqueda->hay = tapa;
    }
}

/* REU "util": mover el tope de una zona destapa un contenedor pedido por los camiones de la ventana de planificacion.
   Una reubicacion que no destapa nada es optativa: el operador puede hacerla cuando se le pide una orden,
   pero no cuenta como operacion pendiente para el avance automatico. */
static int hayReubicacionUtil(tEstado* sis, tCola* camionesEspera, const tConfig* conf)//
{
    tBusquedaReu busqueda;

    busqueda.zonas = &sis->zonas;
    busqueda.cantPedidos = 0;
    busqueda.hay = 0;
    if (sis->tiempo_actual + conf->tiempo_reubicacion <= conf->duracion_jornada)
    {
        busqueda.cantPedidos = ventanaPedidos(camionesEspera, busqueda.pedidos, VER_CAMIONES);
        if (busqueda.cantPedidos > 0)
            RecorrerLista(&sis->zonas, zonaTapaPedido, &busqueda);
    }
    return busqueda.hay;
}

static int hayEntregaValida(tEstado* sis, tCola* camionesEspera, const tConfig* conf)//hay tiempo, un camion al frente y el contenedor que pide esta al tope de alguna zona
{
    tCamion k;
    tZona z;
    char tope[TAM_COD];
    int pos = 0, hay = 0;

    if (sis->tiempo_actual + conf->tiempo_carga <= conf->duracion_jornada
        && ViewFirst(camionesEspera, &k, sizeof(tCamion)))
    {
        while (!hay && VerPosLista(&sis->zonas, &z, sizeof(tZona), pos++) != POS_INVALIDA)
            hay = (z.cantcont > 0 && PeekStack(&z.pilacont, tope, sizeof(tope))
                   && strcmpi(tope, k.codcont) == 0);
    }
    return hay;
}

int hayOperacionValida(tEstado* sis, tCola* camionesEspera, const tConfig* conf)
{
    return hayDescargaValida(sis, conf)
        || hayReubicacionUtil(sis, camionesEspera, conf)
        || hayEntregaValida(sis, camionesEspera, conf);
}

/* ---------- Fin de jornada ---------- */

int finalizacionNormal(tEstado* sis, tCola* buquesEspera, tCola* camionesEspera, const tConfig* conf)
{
    return proximoEvento(sis, conf->duracion_jornada) == -1
        && IsEmptyQueue(buquesEspera)
        && todosMuellesVacios(&sis->muelles)
        && IsEmptyQueue(camionesEspera);
}

int bloqueoOperativo(tEstado* sis, tCola* buquesEspera, tCola* camionesEspera, const tConfig* conf)
{
    return !finalizacionNormal(sis, buquesEspera, camionesEspera, conf)
        && !hayOperacionValida(sis, camionesEspera, conf)
        && proximoEvento(sis, conf->duracion_jornada) == -1;
}

int jornadaFinalizada(tEstado* sis, tCola* buquesEspera, tCola* camionesEspera, const tConfig* conf)
{
    return sis->tiempo_actual >= conf->duracion_jornada
        || finalizacionNormal(sis, buquesEspera, camionesEspera, conf)
        || bloqueoOperativo(sis, buquesEspera, camionesEspera, conf);
}

/* ---------- Avance automatico ---------- */

/* Mientras no haya ninguna operacion de movimiento valida y quede algun evento
   dentro de la jornada, salta al proximo evento y lo procesa. Se repite porque
   un arribo puede no habilitar ninguna operacion (ej: camion que espera un
   contenedor de un buque que todavia no llego). */
void avanzarRelojAuto(tEstado* sis, tCola* buquesEspera, tCola* camionesEspera, tOperador* usuario, const tConfig* conf)
{
    int prox;

    while (!hayOperacionValida(sis, camionesEspera, conf)
           && (prox = proximoEvento(sis, conf->duracion_jornada)) != -1)
    {
        printf("Se avanzo automaticamente %d minuto/s (T=%d)\n", prox - sis->tiempo_actual, prox);
        sis->tiempo_actual = prox;
        procesarEventos(sis, buquesEspera, camionesEspera, usuario);
    }
}


void vaciarSimulacion (tEstado* sim, tCola* camionesEspera, tCola* buquesEspera)
{
    tBuque b;
    tZona z;

    while (DeQueue(&sim->buques_programados, &b, sizeof(tBuque)))
    {
        VaciarCola(&b.contenedores);
    }

    while (DeQueue(buquesEspera, &b, sizeof(tBuque)))
    {
        VaciarCola(&b.contenedores);
    }

    VaciarCola(&sim->camiones_programados);

    VaciarCola(camionesEspera);

    VaciarLista(&sim->muelles);

    while (OutPrinLista(&sim->zonas, &z, sizeof(tZona)) != LISTA_VACIA)
    {
        VaciarPila(&z.pilacont);
    }
}

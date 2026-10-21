#include "../globales/globales.h"
#include "usuario.h"

void iniciarUsuario(tOperador* usuario)
{
    char linea[TAM_MAXIMO_NOMBRE_OP];

    do{
    puts("Ingrese el nombre del operador de la jornada:");
    fgets(linea, TAM_MAXIMO_NOMBRE_OP, stdin);
    } while (!lineaValida(linea));
    linea[strcspn(linea, "\n")] = '\0';
    strcpy(usuario->nombre, linea);
    usuario->puntuacion = 0;
    usuario->buquesDescargados = 0;
    usuario->contenEntregados = 0;
    usuario->cantReubicar = 0;
    usuario->camionesPend = 0;
    usuario->jornadas = 0;
    generarIndice(usuario);
    puts ("Usuario iniciado exitosamente");
}

int lineaValida (const char *linea)
{
    while (*linea != '\0')
    {
        if (!isspace((unsigned char)*linea)) {
            return EXITO;
        }
        linea++;
    }
    return FALLO;
}

void mostrarResumenUser (const tConfig* config, const tEstado* sis, const tOperador* usuario)
{
    puts("");
    puts ("RESUMEN DE LA JORNADA");
    puts("");
    printf ("OPERADOR: %s\n", usuario->nombre);
    printf ("BUQUES DESCARGADOS: %d\n", usuario->buquesDescargados);
    printf ("CONTENEDORES ENTREGADOS: %d\n", usuario->contenEntregados);
    printf ("CAMIONES PENDIENTES: %d\n", usuario->camionesPend);
    printf ("OPERACIONES DE REUBICACION REALIZADAS: %d\n", usuario->cantReubicar);
    puts("");
    printf ("TIEMPO UTILIZADO: %d de %d\n", sis->tiempo_actual, config->duracion_jornada);
    puts("");
    printf ("PUNTUACION FINAL: %d\n", usuario->puntuacion);
}

int mostrarResumenMov (FILE* log)
{
    int i=0;
    char linea[150];
    fseek(log, 0, SEEK_SET);
    puts("RESUMEN DE MOVIMIENTOS");
    puts("");
    while(fgets(linea, 150, log))
    {
        puts(linea);
        i++;
    }
    return i;
}

int cmpIndice(const void* a, const void* b)
{
    const tIndice *ai = (const tIndice *)a;
    const tIndice *bi = (const tIndice *)b;

    return strcmpi(ai->nombre, bi->nombre);
}

void accionGuardarIndice(void *info, unsigned tam, unsigned n, void *params)
{
    FILE *pf = (FILE *)params;
    fwrite(info, tam, 1, pf);
}

void generarIndice (tOperador* usuario)
{
    FILE* index, *ops = NULL;
    tArbolBinBusq arbolIndices;
    tIndice ind;

    strcpy(ind.nombre, usuario->nombre);

    crearArbolBinBusq(&arbolIndices);
    cargarArchivoBinOrdenadoArbolBinBusq(&arbolIndices, "indices.idx", sizeof(tIndice));

    if(!buscarElemArbolBinBusq(&arbolIndices, &ind, sizeof(tIndice),cmpIndice))
    {
        ops = fopen("operadores.dat", "r+b");
        if(!ops)
        {
            ops = fopen ("operadores.dat", "w+b");
            if (!ops)
            {
                fprintf(stderr, "ERROR: NO SE PUDO CREAR EL ARCHIVO\n");
                return;
            }
        }
        fseek(ops, 0, SEEK_END);
        ind.pos = ftell(ops) / sizeof(tOperador);
        fwrite(usuario, sizeof(tOperador), 1, ops);
        insertarArbolBinBusq(&arbolIndices, &ind, sizeof(tIndice), cmpIndice);
        fclose(ops);
    }

    index = fopen("indices.idx", "wb");
    if (index)
    {
        recorrerEnOrdenArbolBinBusq(&arbolIndices, index, accionGuardarIndice);
        fclose(index);
    }
    vaciarArbolBinBusq(&arbolIndices);
}

void actualizarRegUsuario (tOperador* usuario)
{
    tArbolBinBusq arbolIndices;
    tIndice ind;
    FILE* ops;
    tOperador historico;

    strcpy(ind.nombre, usuario->nombre);

    crearArbolBinBusq(&arbolIndices);
    cargarArchivoBinOrdenadoArbolBinBusq(&arbolIndices, "indices.idx", sizeof(tIndice));

    if (buscarElemArbolBinBusq(&arbolIndices, &ind, sizeof(tIndice), cmpIndice))
    {
        ops = fopen ("operadores.dat", "r+b");
        if (ops)
        {
            fseek(ops, ind.pos*sizeof(tOperador), SEEK_SET);
            fread(&historico, sizeof(tOperador), 1, ops);

            historico.puntuacion += usuario->puntuacion;
            historico.buquesDescargados += usuario->buquesDescargados;
            historico.contenEntregados += usuario->contenEntregados;
            historico.camionesPend += usuario->camionesPend;
            historico.cantReubicar += usuario->cantReubicar;
            historico.jornadas++;

            fseek(ops, -1*(long)sizeof(tOperador), SEEK_CUR);
            fwrite(&historico, sizeof(tOperador), 1, ops);

            fclose(ops);
        }
    }
    vaciarArbolBinBusq(&arbolIndices);
}

void mostrarHistorialOperador(const char* nombreBuscado)
{
    FILE* arch = fopen("jornadas.dat", "rb");
    if (!arch) return;

    tJornada cabecera;
    tMovimiento mov;
    int jornada_nro = 1;

    while (fread(&cabecera, sizeof(tJornada), 1, arch) == 1) {

        if (strcmpi(cabecera.nombreOp, nombreBuscado) == 0) {
            printf("\n--- Jornada %d de %s (Puntos: %d) ---\n", jornada_nro, cabecera.nombreOp, cabecera.puntuacion);

            for (int i = 0; i < cabecera.cantidad_movimientos; i++) {
                fread(&mov, sizeof(tMovimiento), 1, arch);
                printf("%s\n", mov.operacion);
            }
            jornada_nro++;
        }
        else {
            long bytes_a_saltar = cabecera.cantidad_movimientos * sizeof(tMovimiento);
            fseek(arch, bytes_a_saltar, SEEK_CUR);
        }
    }
    fclose(arch);
    system("pause");
}

int archivarJornada (FILE* log, tOperador* usuario, int cantMov)
{
    char linea[150];
    tJornada jornada;
    tMovimiento mov;
    FILE* jor;

    jor = fopen("jornadas.dat", "a+b");
    if (!jor)
    {
        fprintf(stderr, "ERROR AL ABRIR ARCHIVO DE JORNADAS\n");
        return FALLO;
    }

    fseek(log, 0, SEEK_SET);
    strcpy(jornada.nombreOp, usuario->nombre);
    jornada.cantidad_movimientos = cantMov;
    jornada.puntuacion = usuario->puntuacion;
    fwrite(&jornada, sizeof(tJornada), 1, jor);

    while(fgets(linea, 150, log))
    {
        strcpy(mov.operacion, linea);
        fwrite(&mov, sizeof(tMovimiento), 1, jor);
    }

    fclose(jor);
    return EXITO;
}

void ranking()
{
    FILE* ops;
    tLista lista;
    tOperador usuario;

    ops = fopen("operadores.dat", "rb");
    if (!ops)
    {
        fprintf(stderr, "ERROR: NO SE PUDO ABRIR EL ARCHIVO DE OPERADORES\n");
        return;
    }

    CrearLista(&lista);

    while (fread(&usuario, sizeof(tOperador), 1, ops))
    {
        if (InsPrinLista(&lista, &usuario, sizeof(tOperador)) != EXITO)
        {
            fprintf(stderr, "ERROR: SIN MEMORIA PARA INSERTAR OPERADOR EN EL RANKING\n");
            VaciarLista(&lista);
            fclose(ops);
            return;
        }
    }

    OrdenarLista(&lista, cmpPuntuacion);

    puts("RANKING DE OPERADORES\n");

    while (OutPrinLista(&lista, &usuario, sizeof(tOperador)) == EXITO)
    {
        printf("OPERADOR: %s\tPUNTUACION ACUMULADA: %d\tJORNADAS REALIZADAS: %d\n", usuario.nombre, usuario.puntuacion, usuario.jornadas);
    }

    VaciarLista(&lista);
    fclose(ops);
    system("pause");
}

int cmpPuntuacion(void* a, void* b)
{
    const tOperador* ap = (tOperador*)a;
    const tOperador* bp = (tOperador*)b;
    return (ap->puntuacion-bp->puntuacion);
}

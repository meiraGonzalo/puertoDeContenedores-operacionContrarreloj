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

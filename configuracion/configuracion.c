#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configuracion.h"
#define TAM_ETIQUETA_MAX 100
#define TAM_LINEA_MAX 150

typedef struct
{
    char etiqueta[TAM_ETIQUETA_MAX];
    int* parametroReal;
} tEtiquetasConfig;

void configuracionIniciar (tConfiguracion* config)
{
    config->duracion_jornada = -1;
    config->cantidad_muelles = -1;
    config->cantidad_zonas = -1;
    config->capacidad_pila = -1;
    config->cantidad_buques = -1;
    config->cantidad_contenedores = -1;
    config->cantidad_camiones = -1;
    config->tiempo_descarga = -1;
    config->tiempo_reubicacion = -1;
    config->tiempo_carga = -1;
}

int configuracionCompleta (const tConfiguracion* config)
{
    if (config->duracion_jornada == -1) return FALLO;
    if (config->cantidad_muelles == -1) return FALLO;
    if (config->cantidad_zonas == -1) return FALLO;
    if (config->capacidad_pila == -1) return FALLO;
    if (config->cantidad_buques == -1) return FALLO;
    if (config->cantidad_contenedores == -1) return FALLO;
    if (config->cantidad_camiones == -1) return FALLO;
    if (config->tiempo_descarga == -1) return FALLO;
    if (config->tiempo_reubicacion == -1) return FALLO;
    if (config->tiempo_carga == -1) return FALLO;
    return EXITO;
}

int cargarConfiguracion (tConfiguracion* config)
{
    FILE* arch;
    tEtiquetasConfig mapa[] = {
        {"duracion_jornada_minutos", &config->duracion_jornada},
        {"cantidad_muelles", &config->cantidad_muelles},
        {"cantidad_zonas_almacenamiento", &config->cantidad_zonas},
        {"capacidad_pila", &config->capacidad_pila},
        {"maximo_buques", &config->cantidad_buques},
        {"maximo_contenedores_por_buque", &config->cantidad_contenedores},
        {"maximo_camiones", &config->cantidad_camiones},
        {"tiempo_descarga_contenedor", &config->tiempo_descarga},
        {"tiempo_reubicacion_contenedor", &config->tiempo_reubicacion},
        {"tiempo_carga_camion", &config->tiempo_carga}
    };
    int valor, i, cant = sizeof(mapa) / sizeof(tEtiquetasConfig);
    char clave[TAM_ETIQUETA_MAX], linea[TAM_LINEA_MAX];

    configuracionIniciar(config);

    arch = fopen (NOMBRE_ARCHIVO_CONFIGURACION, "rt");
    if (!arch)
    {
        fprintf(stderr, "ERROR AL ABRIR EL ARCHIVO DE CONFIGURACION\n");
        return FALLO;
    }

    while (fgets(linea, TAM_LINEA_MAX, arch))
    {
        if (sscanf(linea, "%99[^:]: %d\n", clave, &valor) == 2)
        {
            for (i=0; i<cant; i++)
            {
                if (!strcmpi(clave, mapa[i].etiqueta))
                {
                    *(mapa[i].parametroReal) = valor;
                    break;
                }
            }
        }
    }

    fclose(arch);
    if(!configuracionCompleta(config))
    {
        fprintf(stderr, "NO SE CARGARON TODOS LOS PARAMETROS DE CONFIGURACION\n");
        return FALLO;
    }

    return EXITO;
}

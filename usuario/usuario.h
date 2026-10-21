#ifndef USUARIO_H_INCLUDED
#define USUARIO_H_INCLUDED
#include "../globales/globales.h"

typedef struct{
    char nombre[TAM_MAXIMO_NOMBRE_OP];
    int pos;
} tIndice;

typedef struct {
    char nombreOp[TAM_MAXIMO_NOMBRE_OP];
    int puntuacion;
    int cantidad_movimientos;
} tJornada;

typedef struct {
    char operacion[150];
} tMovimiento;

void iniciarUsuario(tOperador* usuario);
int lineaValida (const char *linea);
void mostrarResumenUser (const tConfig* config, const tEstado* sis, const tOperador* usuario);
int mostrarResumenMov (FILE* log);

int cmpIndice (const void* a, const void* b);
void accionGuardarIndice(void *info, unsigned tam, unsigned n, void *params);
void generarIndice (tOperador* usuario);
void actualizarRegUsuario (tOperador* usuario);
void mostrarHistorialOperador(const char* nombreBuscado);
int archivarJornada (FILE* log, tOperador* usuario, int cantMov);
void ranking();
int cmpPuntuacion(void* a,void* b);

#endif // USUARIO_H_INCLUDED

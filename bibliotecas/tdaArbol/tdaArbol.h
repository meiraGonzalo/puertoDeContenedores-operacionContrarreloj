#ifndef TDAARBOL_H_INCLUDED
#define TDAARBOL_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define CLA_DUP 0
#define SIN_MEM 0
#define SIN_INICIALIZAR 0
#define ERROR_ARCH 0
#define TODO_BIEN 1

typedef struct sNodoArbol
{
    void *info;
    unsigned tamInfo;
    struct sNodoArbol *izq,
                      *der;
} tNodoArbol;

typedef tNodoArbol *tArbolBinBusq;

void crearArbolBinBusq(tArbolBinBusq *p);
int cargarArchivoBinOrdenadoArbolBinBusq(tArbolBinBusq *p, const char *path, unsigned tamInfo);
int buscarElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned tam, int (*cmp)(const void *, const void *));
int insertarArbolBinBusq(tArbolBinBusq *p, const void *d, unsigned tam, int (*cmp)(const void *, const void *));
void recorrerEnOrdenArbolBinBusq(const tArbolBinBusq *p, void *params, void (*accion)(void *, unsigned, unsigned, void *));
void vaciarArbolBinBusq(tArbolBinBusq *p);

int cargarDesdeDatosOrdenadosRec(tArbolBinBusq *p, void *ds, unsigned (*leer)(void **, void *, unsigned, void *params), int li, int ls, void *params);
unsigned leerDesdeArchivoBin(void **d, void *pf, unsigned pos, void *params);
tNodoArbol **buscarNodoArbolBinBusq(const tArbolBinBusq *p, const void *d, int (*cmp)(const void *, const void *));
void recorrerEnOrdenRecArbolBinBusq(const tArbolBinBusq *p, unsigned n, void *params, void (*accion)(void *, unsigned, unsigned, void *));

#endif // TDAARBOL_H_INCLUDED

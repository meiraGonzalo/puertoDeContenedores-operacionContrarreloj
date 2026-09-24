#include "tdaArbol.h"

#define MINIMO(X, Y) ((X)<(Y)?(X):(Y))
#define reservarMemoriaNodo(X, Y, Z, W) ( \
    ((X) = (typeof(X))malloc(Y)) == NULL || \
    ((Z) = malloc(W)) == NULL ? \
    free(X), 0 : 1)

void crearArbolBinBusq(tArbolBinBusq *p)
{
    *p = NULL;
}

int cargarArchivoBinOrdenadoArbolBinBusq(tArbolBinBusq *p, const char *path, unsigned tamInfo)
{
    int cantReg, r;
    FILE *pf;
    if (*p)
        return SIN_INICIALIZAR;
    if (!(pf = fopen(path, "rb")))
        return ERROR_ARCH;
    fseek(pf, 0L, SEEK_END);
    cantReg = ftell(pf) / tamInfo;
    r = cargarDesdeDatosOrdenadosRec(p, pf, leerDesdeArchivoBin, 0, cantReg - 1, &tamInfo);
    fclose(pf);
    return r;
}

int buscarElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned tam, int (*cmp)(const void *, const void *))
{
    if (!(p = (const tArbolBinBusq *)buscarNodoArbolBinBusq(p, d, cmp)))
        return 0; ///NO_EXISTE
    memcpy(d, (*p)->info, MINIMO(tam, (*p)->tamInfo));
    return 1;
}

int insertarArbolBinBusq(tArbolBinBusq *p, const void *d, unsigned tam, int (*cmp)(const void *, const void *))
{
    tNodoArbol *nue;
    int rc;

    while (*p)
    {
        if ((rc = cmp(d, (*p)->info)) < 0)
            p = &(*p)->izq;
        else if (rc > 0)
            p = &(*p)->der;
        else
            return CLA_DUP;
    }

    if (!reservarMemoriaNodo(nue, sizeof(tNodoArbol), nue->info, tam))
        return SIN_MEM;

    nue->tamInfo = tam;
    memcpy(nue->info, d, tam);
    nue->der = nue->izq = NULL;
    *p = nue;
    return TODO_BIEN;
}

void recorrerEnOrdenArbolBinBusq(const tArbolBinBusq *p, void *params, void (*accion)(void *, unsigned, unsigned, void *))
{
    recorrerEnOrdenRecArbolBinBusq(p, 0, params, accion);
}

void vaciarArbolBinBusq(tArbolBinBusq *p)
{
    if (!*p)
        return;

    vaciarArbolBinBusq(&(*p)->izq);
    vaciarArbolBinBusq(&(*p)->der);

    free((*p)->info);
    free(*p);

    *p = NULL;
}

int cargarDesdeDatosOrdenadosRec(tArbolBinBusq *p, void *ds, unsigned (*leer)(void **, void *, unsigned, void *params), int li, int ls, void *params)
{
    int m = (li + ls) / 2, r;
    if (li > ls)
        return TODO_BIEN;

    *p = (tNodoArbol *)malloc(sizeof(tNodoArbol));
    if (!*p || !((*p)->tamInfo = leer(&(*p)->info, ds, m, params)))
    {
        free(*p);
        return SIN_MEM;
    }

    (*p)->izq = (*p)->der = NULL;
    if ((r = cargarDesdeDatosOrdenadosRec(&(*p)->izq, ds, leer, li, m - 1, params)) != TODO_BIEN)
        return r;
    return cargarDesdeDatosOrdenadosRec(&(*p)->der, ds, leer, m + 1, ls, params);
}

unsigned leerDesdeArchivoBin(void **d, void *pf, unsigned pos, void *params)
{
    unsigned tam = *((int *)params);
    *d = malloc(tam);
    if (!*d)
        return 0;
    fseek((FILE *)pf, pos * tam, SEEK_SET);

    if (fread(*d, tam, 1, (FILE *)pf) != 1)
        return 0;

    return tam;
}

tNodoArbol **buscarNodoArbolBinBusq(const tArbolBinBusq *p, const void *d, int (*cmp)(const void *, const void *))
{
    int rc;
    while (*p && (rc = cmp(d, (*p)->info)))
    {
        if (rc < 0)
            p = &(*p)->izq;
        else
            p = &(*p)->der;
    }
    if (!*p)
        return NULL;
    return (tNodoArbol **)p;
}

void recorrerEnOrdenRecArbolBinBusq(const tArbolBinBusq *p, unsigned n, void *params, void (*accion)(void *, unsigned, unsigned, void *))
{
    if (!*p)
        return;
    recorrerEnOrdenRecArbolBinBusq(&(*p)->izq, n + 1, params, accion);
    accion((*p)->info, (*p)->tamInfo, n, params);
    recorrerEnOrdenRecArbolBinBusq(&(*p)->der, n + 1, params, accion);
}

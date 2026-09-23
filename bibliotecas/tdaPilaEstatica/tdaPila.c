#include "tdaPila.h"
#include <string.h>
#define FALLO 0
#define EXITO 1

void crearPila(tPila* p)
{
    p->tope = TAM;
}

void vaciarPila(tPila *p)
{
    p->tope = TAM;
}

int pilaLlena(const tPila *p, unsigned tam)
{
    if (p->tope < tam + sizeof(int))
        return EXITO;
    else
        return FALLO;
}

int pilaVacia(const tPila* p)
{
    if (p->tope == TAM)
        return EXITO;
    else
        return FALLO;
}

int verTope(const tPila* p, void* dato, unsigned tam) // esta funcion no elimina el elemento, consultar si es el comportamiento esperado
{
    if (p->tope == TAM) // verifico que la pila no este vacia
        return FALLO;
    int tamElem, min;
    memcpy(&tamElem, &p->pila[p->tope], sizeof(int)); // guardo el tamaño original del elemento
    min = (tamElem < tam) ? tamElem : tam;
    memcpy(dato, &p->pila[p->tope + sizeof(int)], min);
    return EXITO;
}

int apilar(tPila* p, const void* dato, unsigned tam)
{
    if (p->tope < tam + sizeof(int)) // verifico que haya espacio disponible para el elemento
        return FALLO;
    else
    {
        p->tope -= tam;
        memcpy(&(p->pila[p->tope]), dato, tam);
        p->tope -= sizeof(int);
        memcpy(&p->pila[p->tope], &tam, sizeof(int));
        return EXITO;
    }
}

int desapilar(tPila* p, void* dato, unsigned tam)
{
    if (p->tope == TAM) // verifico que la pila no este vacia
        return FALLO;
    int tamElem, min;

    memcpy(&tamElem, &p->pila[p->tope], sizeof(int));
    p->tope += sizeof(int);
    min = (tamElem < tam) ? tamElem : tam;
    memcpy(dato, &p->pila[p->tope], min);
    p->tope += tamElem;
    return EXITO;
}


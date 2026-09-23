#ifndef TDAPILA_H_INCLUDED
#define TDAPILA_H_INCLUDED
#define TAM 100

typedef struct // consultar si es mejor definir de esta manera o es preferible definir la estructura en el .c
{
    char pila[TAM];
    int tope;
} tPila;
void crearPila(tPila* p);
void vaciarPila(tPila* p);
int pilaVacia(const tPila* p);
int pilaLlena(const tPila* p, unsigned tam);
int apilar(tPila* p, const void* dato, unsigned tam);
int desapilar(tPila* p, void* dato, unsigned tam);
int verTope(const tPila* p, void* dato, unsigned tam);

#endif // TDAPILA_H_INCLUDED

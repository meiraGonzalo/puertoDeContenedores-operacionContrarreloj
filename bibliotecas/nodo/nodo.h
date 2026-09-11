#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

typedef struct sNodo{
    void *info;
    size_t taminfo;
    struct sNodo *sig;
}tNodo;

#endif // NODO_H_INCLUDED

#ifndef TDACOLA_H_INCLUDED
#define TDACOLA_H_INCLUDED

#define TAM_COLA 100

typedef struct{
    char cola[TAM_COLA];
    unsigned pri;
    unsigned ult;
    unsigned tamDisp;
} tCola;

void crearCola(tCola* c);
void vaciarCola (tCola* c);
int colaVacia (const tCola* c);
int colaLlena (const tCola* c, unsigned tamDato);
int acolar (tCola* c, const void* dato, unsigned tamDato);
int desAcolar (tCola* c, void* dato, unsigned tamDato);
int verPrimero (const tCola* c, void* dato, unsigned tamDato);

#endif // TDACOLA_H_INCLUDED

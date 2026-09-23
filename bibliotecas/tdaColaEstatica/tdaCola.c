#include "tdaCola.h"
#include <string.h>
#define EXITO 1
#define FALLO 0

void crearCola(tCola* c) //bien
{
    c->pri = 0;
    c->ult = c->pri;
    c->tamDisp = TAM_COLA;
}

void vaciarCola(tCola* c) //bien
{
    c->ult = c->pri;
    c->tamDisp = TAM_COLA;
}

int colaVacia(const tCola* c) //bien
{
    if (c->tamDisp == TAM_COLA)
        return EXITO;
    return FALLO;
}

int colaLlena(const tCola* c, unsigned tamDato) //bien
{
    if (c->tamDisp < tamDato + sizeof(unsigned))
        return EXITO;
    return FALLO;
}

int verPrimero(const tCola* c, void* dato, unsigned tamDato)
{
    unsigned tamElem, ini, fin, pos = c->pri;

    if (c->tamDisp == TAM_COLA)
        return FALLO; //la cola esta vacia
    ini = (TAM_COLA - pos < sizeof(unsigned)) ? TAM_COLA - pos : sizeof(unsigned);//primera parte: saber el tamaño
    if (ini > 0)
    {
        memcpy(&tamElem, c->cola+pos, ini);
        pos += ini;
    }
    fin = sizeof(unsigned) - ini;
    if (fin > 0)
    {
        memcpy((char*)&tamElem + ini, c->cola, fin);
        pos = fin;
    }
    tamElem = (tamElem < tamDato) ? tamElem : tamDato;
    ini = (TAM_COLA - pos < tamElem) ? TAM_COLA - pos : tamElem;//segunda parte: bajar el dato
    if (ini > 0)
        memcpy(dato, c->cola+pos, ini);
    fin = tamElem - ini;
    if (fin > 0)
        memcpy((char*)dato + ini, c->cola, fin);
    return EXITO;
}

int acolar (tCola* c, const void* dato, unsigned tamDato)
{
    int inicioDato, finDato;

    if (c->tamDisp < tamDato + sizeof(unsigned))
        return FALLO; //cola llena

    c->tamDisp -= tamDato + sizeof(unsigned); //actualizamos tamaño disponible de la cola

    inicioDato = (TAM_COLA - c->ult < sizeof(tamDato)) ? TAM_COLA - c->ult : sizeof(tamDato);//primera parte: guardar  el tamaño
    if (inicioDato > 0)
    {
        memcpy(c->cola+c->ult, &tamDato, inicioDato);
        c->ult += inicioDato;
    }
    finDato = sizeof(tamDato) - inicioDato;
    if (finDato > 0)
    {
        memcpy(c->cola, (char*)&tamDato + inicioDato, finDato);
        c->ult = finDato;
    }

    inicioDato = (TAM_COLA - c->ult < tamDato) ? TAM_COLA - c->ult : tamDato; //segunda parte: guardar el dato
    if (inicioDato > 0)
    {
        memcpy(c->cola+c->ult, dato, inicioDato);
        c->ult += inicioDato;
    }
    finDato = tamDato - inicioDato;
    if (finDato > 0)
    {
        memcpy(c->cola, (char*)dato + inicioDato, finDato);
        c->ult = finDato;
    }

    if (c->ult == TAM_COLA) c->ult = 0; //por seguridad reiniciamos el puntero si quedo en el limite

    return EXITO;
}

int desAcolar(tCola* c, void* dato, unsigned tamDato)
{
    int tamOriginal, tamFinal, ini, fin;

    if (c->tamDisp == TAM_COLA)
        return FALLO; //cola vacia

    ini = (TAM_COLA - c->pri < sizeof(unsigned)) ? TAM_COLA - c->pri : sizeof(unsigned);//primera parte: bajar el tamaño
    if (ini > 0)
    {
        memcpy(&tamOriginal, c->cola+c->pri, ini);
        c->pri += ini;
    }
    fin = sizeof(unsigned) - ini;
    if (fin > 0)
    {
        memcpy((char*)&tamOriginal + ini, c->cola, fin);
        c->pri = fin;
    }

    tamFinal = (tamDato < tamOriginal) ? tamDato : tamOriginal; //el usuario puede desear bajar una cadena mas corta

    ini = (TAM_COLA - c->pri < tamFinal) ? TAM_COLA - c->pri : tamFinal;//segunda parte: bajar el dato
    if (ini > 0)
    {
        memcpy(dato, c->cola+c->pri, ini);
        c->pri += ini;
    }
    fin = tamFinal - ini;
    if (fin > 0)
    {
        memcpy((char*)dato + ini, c->cola, fin);
        c->pri = fin;
    }

    c->pri = (c->pri + (tamOriginal - tamFinal)) % TAM_COLA; // avanzamos pri hasta el siguiente elemento
    c->tamDisp += tamOriginal + sizeof(unsigned); //actualizamos tamaño disponible

    return EXITO;
}

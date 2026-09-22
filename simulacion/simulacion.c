#include "simulacion.h"

int simulacion()
{
    tConfig config;

    system("cls");
    if(!cargarConfiguracion(&config))
    {
        system("pause"); //pausa para ver el mensaje de error
        return FALLO;
    }

    return 0;
}

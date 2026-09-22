#include <stdio.h>
#include <stdlib.h>
#include "menu/menu.h"

int main()
{
    int opcion = STANDBY;

    while (opcion != EXIT)
    {
        menu();
        seleccionarMenu(&opcion);
        dirigir(opcion);
    }

    return 0;
}

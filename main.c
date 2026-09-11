#include "funciones.h"

#define ERROR 1
#define FIN 0

int main()
{
    srand(time(NULL));
    tConfig conf;
    if(!CargarConfiguracion("config.txt", &conf)){
        fprintf(stderr,"ERROR: carga de archivo config.txt no realizada");
        return ERROR;
    }
    if(!GenerarSimulacion("puerto.txt",&conf)){
        fprintf(stderr,"ERROR: generacion de archivo puerto.txt no realizada");
        return ERROR;
    }

    return FIN;
}

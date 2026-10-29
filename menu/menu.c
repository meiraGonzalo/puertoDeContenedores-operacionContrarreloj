#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

#define EXITO 1
#define FALLO 0

void menu()
{
    system("cls");

    printf("===== OPERACION CONTRARRELOJ =====\n");
    puts("");
    printf("1. Iniciar nueva jornada\n");
    printf("2. Ver ranking de operadores\n");
    printf ("3. Mostrar historial de un operador\n");
    printf("4. Salir\n");
    puts("");
    printf ("Seleccione una opcion: ");
}

void seleccionarMenu(int* opcion)
{
    int temp, esValido = FALLO;
    char bufferEntrada [TAM_BUFFER];
    do {
        fgets(bufferEntrada, sizeof(bufferEntrada), stdin);
        if (sscanf(bufferEntrada, "%d", &temp) != 1)
            fprintf(stderr, "ERROR: FORMATO INVALIDO\n");
        else if (temp < SIM || temp > EXIT)
            fprintf(stderr, "ERROR: OPCION INVALIDA EN EL MENU\n");
            else
                esValido = EXITO;
    } while (esValido == FALLO);
    *opcion = temp;
}

void dirigir(int opcion)
{
    char nombre[TAM_MAXIMO_NOMBRE_OP];
    switch(opcion)
    {
    case SIM:
        simulacion();
        break;

    case RANK:
        ranking();
        break;

    case HISTORIAL:
        puts("Ingrese el nombre del operador:");
        fgets(nombre, TAM_MAXIMO_NOMBRE_OP, stdin);
        nombre[strcspn(nombre, "\n")] = '\0';
        mostrarHistorialOperador(nombre);
        break;

    case EXIT:
        puts("");
        puts ("Cerrando aplicacion...\n");
        break;
    }
}

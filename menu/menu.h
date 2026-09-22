#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "../simulacion/simulacion.h"

#define SIM 1
#define RANK 2
#define HISTORIAL 3
#define EXIT 4
#define STANDBY 5
#define TAM_BUFFER 50

void menu (); //muestra las opciones
void seleccionarMenu (int* opcion); //valida que la opcion ingresada sea correcta
void dirigir (int opcion); //dirige a la funcion correspondiente

#endif // MENU_H_INCLUDED

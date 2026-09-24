#include "buque_muelle.h"

int InicializarMuelles(tLista *muelles, int cant_muelles){
    tMuelle muelle;
    memset(&muelle, 0, sizeof(tMuelle)); //inicializamos en 0 toda la estructura muelle para evitar basura
    while(cant_muelles){
        muelle.nro=cant_muelles;
        muelle.estado=1; //al principio todos estan libres
        if(!InsPrinLista(muelles, &muelle, sizeof(muelle)))
        {
            VaciarLista(muelles);
            return ERROR_MALLOC;
        }
        cant_muelles--;
    }
    return EXITO;
}

int CmpEstado(void *a, void *b){ // compara estados entre muelles
    tMuelle *muelleA=(tMuelle*)a;
    tMuelle *muelleB=(tMuelle*)b;
    if(muelleA->estado==muelleB->estado)
        return 0;
    return -1;
}
int AsignarBuques(tCola *buquesProgramados, tLista *muelles, tCola* buquesEspera, int tiempo)
{
    int poslibre;
    tBuque buque;
    tMuelle muelle, muelle_libre, muelleaux;
    muelle_libre.estado = 1;

    while (ViewFirst(buquesEspera, &buque, sizeof(tBuque))) { //primero revisalos si hay buques ya arribados en espera
        poslibre = BuscLista(muelles, &muelle_libre, CmpEstado);
        if (poslibre != POS_INVALIDA) {
            DeQueue(buquesEspera, &buque, sizeof(tBuque));
            muelle.nro = poslibre + 1;
            muelle.asignado = buque;
            muelle.estado = 0;
            OutPosLista(muelles, &muelleaux, sizeof(tMuelle), poslibre);
            InsPosLista(muelles, &muelle, sizeof(tMuelle), poslibre);
        } else {
            break;
        }
    }

    while (ViewFirst(buquesProgramados, &buque, sizeof(tBuque)) && buque.tiempo <= tiempo) { //continuamos con los buques que estan arribando
        DeQueue(buquesProgramados, &buque, sizeof(tBuque));

        poslibre = BuscLista(muelles, &muelle_libre, CmpEstado);

        if (poslibre != POS_INVALIDA) {
            muelle.nro = poslibre + 1;
            muelle.asignado = buque;
            muelle.estado = 0;
            OutPosLista(muelles, &muelleaux, sizeof(tMuelle), poslibre);
            InsPosLista(muelles, &muelle, sizeof(tMuelle), poslibre);
        } else {
            EnQueue(buquesEspera, &buque, sizeof(tBuque));
        }
    }

    return EXITO;
}

void MostrarMuelle(void *info, void *sin_usar){
    char codcont[TAM_COD];
    tMuelle *muelle=(tMuelle*)info;
    if(muelle->estado==0){
    tBuque buque=muelle->asignado;
    ViewFirst(&(buque.contenedores), codcont, sizeof(codcont));
    printf("M%d: %s --> proximo contenedor: %s \n",muelle->nro, buque.cod,codcont);
    }
}

void LiberarVacios(void *infoMuelle, void *usuario)
{
    tMuelle *muelle=(tMuelle*)infoMuelle;
    tOperador* user = (tOperador*)usuario;
    if(muelle->estado==0 && IsEmptyQueue(&((muelle->asignado).contenedores)))
    {
        muelle->estado=1;
        user->buquesDescargados++;
        user->puntuacion +=5;
    }
}
void EmbarcarBuquesVacios(tLista *muelles, tOperador* usuario){
    RecorrerLista(muelles, LiberarVacios, usuario);
}

int asignarCamiones(tEstado* sis, tCola* camionesEspera)
{
    tCamion camion;

    while(!IsEmptyQueue(&sis->camiones_programados))
    {
        ViewFirst(&sis->camiones_programados, &camion, sizeof(tCamion));
        if (camion.tiempo <= sis->tiempo_actual)
        {
            if(!EnQueue(camionesEspera, &camion, sizeof(tCamion)))
            {
                fprintf(stderr, "ERROR: SIN MEMORIA PARA COLA DE ESPERA DE CAMIONES\n");
                return FALLO;
            }
            DeQueue(&sis->camiones_programados, &camion, sizeof(tCamion));
        }
        else
            break;
    }
    return EXITO;
}

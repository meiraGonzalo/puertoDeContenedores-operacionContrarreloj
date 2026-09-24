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
int AsignarBuques(tCola *buques, tLista *muelles, int tiempo){
    int poslibre=0, hay_encolados;
    tBuque buque;
    tMuelle muelle,muelle_libre, muelleaux;
    muelle_libre.estado=1;
    if(!(hay_encolados=ViewFirst(buques, &buque, sizeof(tBuque)))){
        return COLA_VACIA;
    }
    while(buque.tiempo<=tiempo && poslibre!=-1 && hay_encolados){
        if((poslibre=BuscLista(muelles, &muelle_libre, CmpEstado))!=-1){ //encontrara el primer muelle libre
            DeQueue(buques, &buque,sizeof(tBuque));
            muelle.nro=poslibre+1;
            muelle.asignado=buque;
            muelle.estado=0;
            OutPosLista(muelles, &muelleaux, sizeof(tMuelle),poslibre); //quitamos el nodo que estaba en pos
            InsPosLista(muelles, &muelle, sizeof(tMuelle),poslibre);// insertamos el nuevo nodo con los datos del muelle
        }
        hay_encolados=ViewFirst(buques, &buque, sizeof(tBuque));
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

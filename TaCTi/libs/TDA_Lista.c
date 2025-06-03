#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h> // para rand() y srand()
#include <time.h>   // para time()


#include "TDA_Lista.h"


void crearLista(tLista *l)
{
    *l = NULL;
}

int listaVacia(tLista *l)
{
    return *l == NULL;
}

int listaLlena(tLista *l, const void *dato, unsigned tamDato)
{
    tNodo *nue;
    if(!l || !dato || tamDato <= 0)
        return ERROR;

    nue = (tNodo *)malloc(sizeof(tNodo));
    if(!nue)
        return ERROR;
    nue->info = (void *)malloc(tamDato);
    if(!nue->info)
    {
        free(nue);
        return ERROR;
    }
    return OK;
}

int vaciarLista(tLista *l)
{
    tNodo *aux;

    if(!l)
        return ERROR;

    while(*l)
    {
        aux = *l;
        *l = aux->sig;
        free(aux->info);
        free(aux);
    }
    return OK;
}

int ponerAlPrincipio(tLista *l, const void *dato, unsigned tamDato)
{
    tNodo *nue;
    if(!l || !dato || tamDato <= 0)
        return ERROR;

    nue = (tNodo *)malloc(sizeof(tNodo));
    if(!nue)
        return ERROR;
    nue->info = (void *)malloc(tamDato);
    if(!nue->info)
    {
        free(nue);
        return ERROR;
    }

    memcpy(nue->info,dato,tamDato);
    nue->tamInfo = tamDato;

    nue->sig = *l;
    *l = nue;
    return OK;
}

int verAlPrincipio(tLista *l, void *dato, unsigned tamDato)
{
    if(!l || !dato || tamDato <= 0)
        return ERROR;

    if(*l == NULL)
        return ERROR;

    memcpy(dato,(*l)->info,MIN(tamDato,(*l)->tamInfo));
    return OK;
}

int sacarAlPrincipio(tLista *l, void *dato, unsigned tamDato)
{
    tNodo *aux;
    if(!l || !dato || tamDato <= 0 || (*l) == NULL)
        return ERROR;

    aux = *l;
    memcpy(dato,aux->info,MIN(tamDato,aux->tamInfo));
    *l = aux->sig;
    free(aux->info);
    free(aux);
    return OK;
}

int ponerAlFinal(tLista *l, void *dato, unsigned tamDato)
{
    tNodo *nue;
    if(!l || !dato || tamDato <= 0)
        return ERROR;
    nue = (tNodo *)malloc(sizeof(tNodo));
    if(!nue)
        return ERROR;
    nue->info = (void *)malloc(tamDato);
    if(!nue->info)
    {
        free(nue);
        return ERROR;
    }

    while(*l)
        l = &(*l)->sig;

    memcpy(nue->info,dato,tamDato);
    nue->tamInfo = tamDato;
    nue->sig = NULL;
    *l = nue;
    return OK;
}

int verAlFinal(tLista *l, void *dato, unsigned tamDato)
{
    if(!l || !dato || tamDato <= 0)
        return ERROR;
    if(*l == NULL)
        return ERROR;

    while((*l)->sig != NULL)
        l = &(*l)->sig;

    memcpy(dato,(*l)->info,MIN(tamDato,(*l)->tamInfo));
    return OK;
}

int sacarAlFinal(tLista *l, void *dato, unsigned tamDato)
{
    if(!l || !dato || tamDato <= 0)
        return ERROR;

    if(*l == NULL)
        return ERROR;

    while((*l)->sig != NULL)
        l = &(*l)->sig;

    memcpy(dato, (*l)->info,MIN(tamDato, (*l)->tamInfo));
    free((*l)->info);
    free((*l));
    *l = NULL;
    return OK;
}


void recorrerLista(const tLista *p,
                   unsigned cantBytes,
                   void (*mostrar)(const void *))
{
    void *temp;
    int min;

    while(*p)
    {
        min = MIN(cantBytes, (*p)->tamInfo);
        temp = malloc(min);
        memcpy(temp, (*p)->info, min);
        mostrar(temp);
        free(temp);
        p = &(*p)->sig;
    }
}



void desordenarLista(tLista *p) {
    if (!p || !*p || !(*p)->sig)
        return;

    // Contar nodos
    int n = 0;
    tNodo *actual = *p;
    while (actual) {
        n++;
        actual = actual->sig;
    }

    // Crear arreglo de punteros a nodos
    tNodo **nodos = malloc(n * sizeof(tNodo*));
    if (!nodos)
        return;

    actual = *p;
    for (int i = 0; i < n; i++) {
        nodos[i] = actual;
        actual = actual->sig;
    }

    // Mezclar arreglo aleatoriamente
    srand((unsigned)time(NULL));  // Semilla
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        tNodo *tmp = nodos[i];
        nodos[i] = nodos[j];
        nodos[j] = tmp;
    }

    // Reconectar la lista con el nuevo orden
    for (int i = 0; i < n - 1; i++) {
        nodos[i]->sig = nodos[i + 1];
    }
    nodos[n - 1]->sig = NULL;
    *p = nodos[0];

    free(nodos);
}





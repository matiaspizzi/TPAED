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
    *l = NULL;
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


void recorrerMostrarLista(const tLista *p,
                   unsigned cantBytes,
                   void (*mostrar)(const void *))
{
    while(*p)
    {
        mostrar((*p)->info);
        p = &(*p)->sig;
    }
}


void recorrerGuardandoLista(const tLista *p,
                   unsigned cantBytes,
                   void *dato, unsigned tamDato)
{
    int min;

    while(*p)
    {
        min = MIN(cantBytes, (*p)->tamInfo);
        memcpy(dato, (*p)->info, min);
        p = &(*p)->sig;
    }
}

int verEnPosicion(tLista *l, unsigned pos, void *dato, unsigned tamDato)
{
    unsigned i = 0;
    tNodo *act;

    if (!l || !*l || !dato)
        return ERROR;

    act = *l;

    while (act && i < pos)
    {
        act = act->sig;
        i++;
    }

    if (act)
    {
        memcpy(dato, act->info, MIN(tamDato, act->tamInfo));
        return OK;
    }

    return ERROR;
}


void ordenarListaAleatorio(tLista *l)
{
    tNodo **nodos = NULL;
    unsigned qty = 0, i;
    tNodo *act = *l;

    // 1. Contar nodos
    while (act)
    {
        qty++;
        act = act->sig;
    }

    if (qty < 2)
        return;  // No hay nada que mezclar

    // 2. Crear array de punteros
    nodos = malloc(qty * sizeof(tNodo *));
    if (!nodos)
        return;  // Error de memoria

    act = *l;
    for (i = 0; i < qty; i++)
    {
        nodos[i] = act;
        act = act->sig;
    }

    // 3. Barajar el array (Fisher-Yates)
    for (i = qty - 1; i > 0; i--)
    {
        unsigned j = rand() % (i + 1);
        tNodo *tmp = nodos[i];
        nodos[i] = nodos[j];
        nodos[j] = tmp;
    }

    // 4. Reconstruir la lista
    *l = nodos[0];
    for (i = 0; i < qty - 1; i++)
    {
        nodos[i]->sig = nodos[i + 1];
    }
    nodos[qty - 1]->sig = NULL;

    // 5. Liberar array auxiliar
    free(nodos);
}


#include "lista.h"

#define MINIMO( X, Y ) ( ( X ) <= (  Y ) ? ( X ) : ( Y ) )

void crearLista (tLista *p)
{
    *p = NULL;
}

int listaVacia (const tLista *p)
{
    return *p == NULL;
}

int listaLlena (const tLista *p, unsigned cantBytes)
{
    tNodo *aux = (tNodo *)malloc(sizeof(tNodo));
    void *info = malloc(cantBytes);
    free(aux);
    free(info);
    return aux == NULL || info == NULL;
}

void vaciarLista (tLista *p)
{
    while (*p)
    {
        tNodo* aux = *p;
        *p = aux->sig;
        free(aux->info);
        free(aux);
    }
}

int ponerAlComienzo (tLista *p, const void* d, unsigned cantBytes)
{
    tNodo *nue = (tNodo*)malloc(sizeof(tNodo));
    if (nue)
        return SIN_MEM;
    nue->info = malloc(sizeof(cantBytes));
    if (nue->info)
    {
        free(nue);
        return SIN_MEM;
    }
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = *p;
    *p = nue;
    return TODO_BIEN;
}

int sacarPrimeroLista (tLista *p, void *d, unsigned cantBytes)
{
    tNodo *aux = *p;
    if(aux == NULL)
        return 0;
    *p = aux->sig;
    memcpy(d, aux->info, MINIMO(cantBytes, aux->tamInfo));
    free(aux->info);
    free(aux);
    return 1;
}

int verPrimeroLista (const tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    return 1;
}

int ponerAlFinal (tLista *p, const void* d, unsigned cantBytes)
{
    tNodo *nue;
    while(*p)
        p = &(*p)->sig;
    if((nue = (tNodo *)malloc(sizeof(tNodo))) == NULL ||
       (nue->info = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return 0;
    }
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = NULL;
    *p = nue;
    return 1;
}

int eliminarUltimoLista (tLista* p)
{
    if(*p == NULL)
        return 0;
    while((*p)->sig)
        p = &(*p)->sig;
    free((*p)->info);
    free(*p);
    *p = NULL;
    return 1;

}
int sacarUltimoLista (tLista* p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    while((*p)->sig)
        p = &(*p)->sig;
    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    free((*p)->info);
    free(*p);
    *p = NULL;
    return 1;
}

int verUltimoLista(const tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    while((*p)->sig)
        p = &(*p)->sig;
    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    return 1;
}



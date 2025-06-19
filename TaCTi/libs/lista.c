/* --------------------------------o---x---o--------------------------------
 *          lista.c     definición y primitivas del TDA LISTA
 *                      implementada en lista dinámica simplemente enlazada
 * --------------------------------o---x---o-------------------------------- */

#include "lista.h"





 void crearLista(tLista *p)
{
    *p = NULL;
}


int  listaVacia(const tLista *p)
{
    return *p == NULL;
}


int  listaLlena(const tLista *p, unsigned cantBytes)
{
    tNodo  *aux = (tNodo *)malloc(sizeof(tNodo));
    void   *info = malloc(cantBytes);

    free(aux);
    free(info);
    return aux == NULL || info == NULL;
}


void vaciarLista(tLista *p)
{
    while(*p)
    {
        tNodo *aux = *p;

        *p = aux->sig;
        free(aux->info);
        free(aux);
    }
}


int  ponerAlComienzo(tLista *p, const void *d, unsigned cantBytes)
{
    tNodo *nue;

    if((nue = (tNodo *)malloc(sizeof(tNodo))) == NULL ||
       (nue->info = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return 0;
    }
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = *p;
    *p = nue;
    return 1;
}


int  sacarPrimeroLista(tLista *p, void *d, unsigned cantBytes)
{
    tNodo *aux = *p;

    if(aux == NULL)
        return 0;
    *p = aux->sig;
    memcpy(d, aux->info, MIN(cantBytes, aux->tamInfo));
    free(aux->info);
    free(aux);
    return 1;
}


int  verPrimeroLista(const tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    memcpy(d, (*p)->info, MIN(cantBytes, (*p)->tamInfo));
    return 1;
}


int  ponerAlFinal(tLista *p, const void *d, unsigned cantBytes)
{
    tNodo  *nue;

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


int  sacarUltimoLista(tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    while((*p)->sig)
        p = &(*p)->sig;
    memcpy(d, (*p)->info, MIN(cantBytes, (*p)->tamInfo));
    free((*p)->info);
    free(*p);
    *p = NULL;
    return 1;
}


int  verUltimoLista(const tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    while((*p)->sig)
        p = &(*p)->sig;
    memcpy(d, (*p)->info, MIN(cantBytes, (*p)->tamInfo));
    return 1;
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




int insertarEnOrden(tLista *p,
                    const void *d,
                    unsigned cantBytes,
                    int (*comparar)(const void *, const void *),
                    void (*acumular)(void **, unsigned *,const void *, unsigned))
{
    tNodo *nue;

    while(*p && comparar((*p)->info, d) < 0)
            p = &(*p)->sig;

    if(*p && comparar((*p)->info, d) == 0)
    {
        if(acumular)
        {
            acumular(&(*p)->info, &(*p)->tamInfo, d, cantBytes);
        }
        return CLA_DUP;
    }

    if((nue = (tNodo *)malloc(sizeof(tNodo))) == NULL ||
       (nue->info = malloc(cantBytes)) == NULL)
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

void ordenar(tLista *p, int (*comparar)(const void *, const void *))
{
    tLista *pri = p;

    if (*p == NULL)
        return;

    while ((*p)->sig)
    {
        if (comparar((*p)->info, (*p)->sig->info) < 0)
        {
            tLista *q = pri;
            tNodo *aux = (*p)->sig;

            (*p)->sig = aux->sig;
            while (comparar((*q)->info, aux->info) > 0)
                q = &(*q)->sig;
            aux->sig = *q;
            *q = aux;
        }
        else
        {
            p = &(*p)->sig;
        }
    }
}

int compararEnteros(const void *a, const void *b)
{
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return va - vb;
}


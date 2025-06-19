/* --------------------------------o---x---o--------------------------------
 *          lista.h     declaración y primitivas del TDA LISTA
 *                      implementada en lista dinámica simplemente enlazada
 * --------------------------------o---x---o-------------------------------- */

#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include "cola.h"

#define     SIN_MEM         1
#define     CLA_DUP         2
#define     TODO_BIEN       0
#define     OK              1
#define     ERROR           0

 #define    MIN( X , Y )     ( ( X ) <= ( Y ) ? ( X ) : ( Y ) )

typedef tNodo *tLista;


void    crearLista                  (tLista *p);
int     listaVacia                  (const tLista *p);
int     listaLlena                  (const tLista *p, unsigned cantBytes);
void    vaciarLista                 (tLista *p);
int     ponerAlComienzo             (tLista *p, const void *d, unsigned cantBytes);
int     sacarPrimeroLista           (tLista *p, void *d, unsigned cantBytes);
int     verPrimeroLista             (const tLista *p, void *d, unsigned cantBytes);
int     ponerAlFinal                (tLista *p, const void *d, unsigned cantBytes);
int     sacarUltimoLista            (tLista *p, void *d, unsigned cantBytes);
int     verUltimoLista              (const tLista *p, void *d, unsigned cantBytes);
void    recorrerLista               (const tLista *p,
                                    unsigned cantBytes,
                                    void (*mostrar)(const void *));

int     insertarEnOrden             (tLista *p, const void *d, unsigned cantBytes,
                                    int (*comparar)(const void *, const void *),
                                    void (*acumular)(void **, unsigned *,const void *, unsigned));

void    ordenarListaAleatorio       (tLista *l);
int     verEnPosicion               (tLista *l, unsigned pos, void *dato, unsigned tamDato);
void    ordenar                     (tLista *p, int (*comparar)(const void *, const void *));

int     compararEnteros             (const void *a, const void *b);
#endif // LISTA_H_INCLUDED

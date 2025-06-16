#ifndef TDA_LISTA_H_INCLUDED
#define TDA_LISTA_H_INCLUDED

#define ERROR       0
#define OK          1

#define MIN(x,y)        ( (x)<(y) ? (x) : (y) )
#define MAX(x,y)        ( (x)>(y) ? (x) : (y) )

typedef struct sNodo
{
    void *info;
    unsigned tamInfo;
    struct sNodo *sig;
}tNodo;

typedef tNodo *tLista;

void    crearLista                  (tLista* l);
int     listaVacia                  (tLista *l);
int     listaLlena                  (tLista *l, const void *dato, unsigned tamDato);
int     vaciarLista                 (tLista *l);
int     ponerAlPrincipio            (tLista *l, const void *dato, unsigned tamDato);
int     verAlPrincipio              (tLista *l, void *dato, unsigned tamDato);
int     sacarAlPrincipio            (tLista *l, void *dato, unsigned tamDato);
int     ponerAlFinal                (tLista *l, void *dato, unsigned tamDato);
int     verAlFinal                  (tLista *l, void *dato, unsigned tamDato);
int     sacarAlFinal                (tLista *l, void *dato, unsigned tamDato);



void recorrerMostrarLista(const tLista *p,
                   unsigned cantBytes,
                   void (*mostrar)(const void *));


void recorrerGuardandoLista(const tLista *p,
unsigned cantBytes,
void *dato, unsigned tamDato);


void desordenarLista(tLista *p);

#endif // TDA_LISTA_H_INCLUDED

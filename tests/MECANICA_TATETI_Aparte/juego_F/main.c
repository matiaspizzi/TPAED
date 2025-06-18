#include <stdio.h>
#include <stdlib.h>
#include "../lista/lista.h"
#include "../tateti_matriz_F/tateti_matriz_F.h"

int main()
{
    tLista p_humano;
    tLista p_pc;
    crearLista(&p_humano);
    crearLista(&p_pc);

    int matriz[3][3] = {
                        { 0, 0,0 },
                        { 0, 0,0 },
                        { 0, 0,0 }
                       };
    iniciar_partida(matriz, &p_humano, &p_pc);

    vaciarLista(&p_humano);
    vaciarLista(&p_pc);
    printf("Hello world!\n");
    return 0;
}

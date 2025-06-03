#include <stdio.h>
#include <stdlib.h>
#include "..\tateti\tateti.h"

#define TODO_OK 0
#define EMPATE 3
int main()
{
    tJugador player_1;
    tJugador player_2;
    int numeros[9];
    iniciar_partida(&player_1, &player_2, numeros);
    int ganador = partida(&player_1, &player_2, numeros);
    if (ganador == EMPATE)
    {
        printf("EMPATE\n");
        return TODO_OK;
    }
    if(ganador == player_1.num_jugador)
    {
        printf("GANADOR: %s\n",player_1.nombre);
        return TODO_OK;
    }
    printf("GANADOR: %s\n",player_2.nombre);
    return 0;
}

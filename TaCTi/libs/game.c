#include <stdlib.h>  // rand, srand
#include <time.h>    // time
#include <math.h>  // Para pow
#include <stdio.h>
#include "game.h"
#include "screens.h"

void init_match(tPlays *p)
{
    crearLista(&p->human);
    crearLista(&p->pc);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = 0;

    p->pc_symbol = fifty_fifty();
    if(p->pc_symbol == XSYM)
        p->human_symbol = OSYM;
    else
        p->human_symbol = XSYM;

    if (fifty_fifty() == 1)
    {
        printf("Arranca PC\n");
        p->curr_symbol = p->pc_symbol;
        pc_playing(board,p);
    }
    else
    {
        printf("Arranca Humano\n");
        p->curr_symbol = p->human_symbol;
    }
}

int human_playing(int board[3][3], tPlays *p, int i, int j)
{
    if (full_board(board))
    {
        return DRAW;     // Empate, no quedan más movimientos.
    }

    // Validar que la casilla esté libre
    if (i < 0 || i > 2 || j < 0 || j > 2 || board[i][j] != 0)
    {
        // Movimiento inválido, se puede manejar con un código especial o repetir turno
        // o algún código para que el jugador elija otra casilla.
        return HUMAN_PLAY;
    }

    // Marca la jugada del humano (valor 1)
    board[i][j] = 1;

    int valor = (i + 1) * (int)pow(10, j);
    ponerAlFinal(&p->human, (void*)&valor, sizeof(int));

    if (check_tateti(&p->human) == 1)
    {
        // Humano gana
        return HUMAN_WIN;
    }

    // Continúa el juego, turno PC
    return PC_PLAY;
}

int pc_playing(int board[3][3], tPlays *p)
{
    int i, j;

    // 1. Intentar ganar
    if (traverse_tateti(board, p, PC_WIN, &i, &j))
    {
        board[i][j] = p->pc_symbol;
        int valor = (i + 1) * (int)(pow(10, j));
        ponerAlFinal(&p->pc, &valor, sizeof(int));
        return PC_WIN;
    }

    // 2. Intentar bloquear al humano
    if (traverse_tateti(board, p, HUMAN_WIN, &i, &j))
    {
        board[i][j] = p->pc_symbol;
        int valor = (i + 1) * (int)(pow(10, j));
        ponerAlFinal(&p->pc, &valor, sizeof(int));
        return HUMAN_PLAY;
    }

    // 3. Elegir aleatorio
    if (randomPosition(board, p, &i, &j) == 0)
    {
        return DRAW;
    }

    board[i][j] = p->pc_symbol;
    int valor = (i + 1) * (int)(pow(10, j));
    ponerAlFinal(&p->pc, &valor, sizeof(int));

    return HUMAN_PLAY;
}

int check_tateti(tLista* p)
{
    static int vecAnswers[8] = { 6, 60, 111, 123, 222, 321, 333, 600 };
    tLista pri = *p;
    tLista i = *p;
    tLista j;
    tLista k;

    while ( i != NULL && i->sig != NULL )
    {
        j = i->sig;

        while ( j != NULL && j->sig != NULL)
        {
            k = j->sig;
            while ( k != NULL)
            {
                if(make_row(* ((int*) i->info) + * ((int*) j->info) + * ((int*) k->info), vecAnswers) == 1)
                {
                    *p = pri;
                    return 1;
                }
                k = k->sig;
            }
            j = j->sig;
        }
        i = i->sig;
    }
    *p = pri;
    return 0;
}


int make_row(int magicNum, int* vecAnswers)
{
    for(int i = 0; i < 8; i++)
    {
        if (vecAnswers[i] == magicNum)
        {
            return 1;
        }
    }
    return 0;
}



int full_board(int board[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == 0)
                return 0;  // Todavía hay al menos un lugar libre
        }
    }
    return 1;  // No hay lugares libres → matriz completa
}

int randomPosition(int board[3][3], tPlays *p, int *iOut, int *jOut)
{
    int libres[9][2];  // Guardamos coordenadas posibles
    int count = 0;

    // Buscar posiciones libres
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == 0)
            {
                libres[count][0] = i;
                libres[count][1] = j;
                count++;
            }
        }
    }

    if (count == 0)
        return 0;  // No hay lugar

    // Elegir una posición libre aleatoria
    srand(time(NULL));  // Inicializar semilla (ideal hacerlo una sola vez en main)
    int pos = rand() % count;
    int i = libres[pos][0];
    int j = libres[pos][1];

    *iOut = i;
    *jOut = j;

    /*// Generar valor estilo (i+1) * 10^j
    int valor = (i + 1) * (int)(pow(10, j));
    ponerAlFinal(&p->pc, &valor, sizeof(int));*/

    return 1;  // Jugada exitosa
}


int traverse_tateti(int board[3][3], tPlays *p, int winTo, int *out_i, int *out_j)
{
    int valor;
    int drop;
    if (winTo == PC_WIN)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == 0)
                {
                    valor = (i + 1) * (pow(10, j));
                    ponerAlFinal(&p->pc, (void*)&valor, sizeof(int));
                    if (check_tateti(&p->pc) == 1)
                    {
                        *out_i = i;
                        *out_j = j;
                        return 1; // encontró jugada ganadora
                    }
                    sacarAlFinal(&p->pc, &drop, sizeof(int));
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == 0)
                {
                    valor = (i + 1) * (pow(10, j));
                    ponerAlFinal(&p->human, (void*)&valor, sizeof(int));
                    if (check_tateti(&p->human) == 1)
                    {
                        *out_i = i;
                        *out_j = j;
                        return 1; // encontró jugada bloqueadora
                    }
                    sacarAlFinal(&p->human, &drop, sizeof(int));
                }
            }
        }
    }
    return 0;
}



int check_tateti_board(int board[3][3], int jugador)
{
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] == jugador && board[i][1] == jugador && board[i][2] == jugador)
            return 1;
        if (board[0][i] == jugador && board[1][i] == jugador && board[2][i] == jugador)
            return 1;
    }
    if (board[0][0] == jugador && board[1][1] == jugador && board[2][2] == jugador)
        return 1;
    if (board[0][2] == jugador && board[1][1] == jugador && board[2][0] == jugador)
        return 1;

    return 0;
}



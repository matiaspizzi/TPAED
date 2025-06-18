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
    }
    else
    {
        printf("Arranca Humano\n");
        p->curr_symbol = p->human_symbol;
    }
}

int human_playing(int board[3][3], tPlays *p, int i, int j)
{
    int valor;
    /*if (full_board(board))
    {
        return DRAW;     // Empate, no quedan más movimientos.
    }*/

    // Validar que la casilla esté libre
    if (i < 0 || i > 2 || j < 0 || j > 2 || board[i][j] != 0)
    {
        // Movimiento inválido
        return HUMAN_PLAY;
    }

    // Marca la jugada del humano
    board[i][j] = p->human_symbol;
    valor = (i + 1) * (int)pow(10, j);
    ponerAlFinal(&p->human, (void*)&valor, sizeof(int));

    if (check_tateti(&p->human) == 1)
    {
        // Humano gana
        return HUMAN_WIN;
    }
    //return pc_playing(int board[3][3], tPlays *p);
    // Continúa el juego, turno PC
    return PC_PLAY;
}

int pc_playing(int board[3][3], tPlays *p)
{
    int sacarHumano;
    int row,col;
    // 1. Intentar ganar
    if (traverse_tateti(board, &p->pc,&row,&col))
    {
        board[row][col] = p->pc_symbol;
        return PC_WIN;
    }
    else if (traverse_tateti(board, &p->human, &row, &col))
    {
        // 2. Intentar bloquear al humano
        board[row][col] = p->pc_symbol;
        sacarAlFinal(&p->human,&sacarHumano,sizeof(int));
        ponerAlFinal(&p->pc, &sacarHumano, sizeof(int));
    }
    else
    {
        if(randomPosition(board, &p->pc, p->pc_symbol) == 0)
            return DRAW;
    }
    return HUMAN_PLAY;
}

int check_tateti(tLista* p)
{
    static int vecAnswers[8] = { 6, 60, 111, 123, 222, 321, 333, 600 };
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
                    return WIN;
                }
                k = k->sig;
            }
            j = j->sig;
        }
        i = i->sig;
    }
    return ERROR;
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

int randomPosition(int  board[3][3], tLista* p_pc, int symbol)
{
    int valor;
    int a = 0;
    int ai = 0;
    int aj = 0;
    /// a = Se fija si hubo algún '0'

    /// ai y aj son las filas y columnas del '0' seleccionado.

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if ( board[i][j] == 0)
            {
                if ( a == 0 )
                {
                    a = 1;
                    ai = i;
                    aj = j;
                }
                /// Elige aleatoriamente si, un elemento que es '0', pasa a ser el nuevo seleccionado.
                if ( rand() % 2 == 0)
                {
                    ai = i;
                    aj = j;
                }
            }
        }
    }
    if ( a != 0)
    {
        /// El último seleccionado pasa a ser un '2' (PC)
        board[ai][aj] = symbol;

        /// Incluye el valor en la Lista PC.
        valor = (ai + 1)* ( pow(10, aj));
        ponerAlFinal(p_pc,(void*)&valor, sizeof(int));

        if(full_board(board))
        {
            return 0;
        }

        return 1;
    }
    /// Retorna 0 (Es decir, NO pudo elegir aleatorio (matriz llena) y es EMPATE
    return 0;
}

int traverse_tateti(int board[3][3], tLista *p, int *row, int *col)
{
    int valor;
    int drop;
    int i,j;
    for(i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (board[i][j] == 0)
            {
                valor = (i + 1)* ( pow(10, j));
                ponerAlFinal(p, (void*)&valor, sizeof(int));
                if(check_tateti(p) == 1)
                {
                    *row = i;
                    *col = j;
                    return 1;
                }
                sacarAlFinal(p,&drop,sizeof(int));
            }
        }

    }
    return 0;
}




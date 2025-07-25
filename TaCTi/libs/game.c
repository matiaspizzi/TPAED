#include <stdlib.h>
#include <time.h>
#include <math.h>
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
        printf("Arranca la PC\n");
        p->curr_symbol = p->pc_symbol;
    }
    else
    {
        printf("Arranca el jugador\n");
        p->curr_symbol = p->human_symbol;
    }
}

int human_playing(int board[3][3], tPlays *p, int i, int j)
{
    int valor;

    // Validar que la casilla este libre
    if (i < 0 || i > 2 || j < 0 || j > 2 || board[i][j] != 0)
    {
        // Movimiento invalido
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

    //  Turno PC
    return PC_PLAY;
}

int pc_playing(int board[3][3], tPlays *p)
{
    int sacarHumano;
    int row,col;
    //  Intentar ganar
    if (traverse_tateti(board, &p->pc,&row,&col))
    {
        board[row][col] = p->pc_symbol;
        printf("--> PC: [%d][%d]\n", row,col);
        return PC_WIN;
    }
    else if (traverse_tateti(board, &p->human, &row, &col))
    {
        //  Intentar bloquear al humano
        board[row][col] = p->pc_symbol;
        printf("--> PC: [%d][%d]\n", row,col);
        sacarUltimoLista(&p->human,&sacarHumano,sizeof(int));
        ponerAlFinal(&p->pc, &sacarHumano, sizeof(int));
    }
    else
    {
        if(randomPosition(board, &p->pc, p->pc_symbol) == 0)
        {
            if(check_tateti(&p->pc))
                return PC_WIN;
            if (full_board(board))
                return DRAW;
        }

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
                return ERROR;  //   Hay al menos un lugar libre
        }
    }
    return OK;  // No hay lugares libres
}

int randomPosition(int  board[3][3], tLista* p_pc, int symbol)
{
    int valor;
    int a = 0;
    int ai = 0;
    int aj = 0;
    // a = Se fija si hubo algún '0'

    // ai y aj son las filas y columnas del '0' seleccionado.

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
                // Elige aleatoriamente si, un elemento que es '0', pasa a ser el nuevo seleccionado.
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
        // El último seleccionado pasa a ser un '2' (PC)
        board[ai][aj] = symbol;
        printf("--> PC: [%d][%d]\n", ai,aj);
        // Incluye el valor en la Lista PC.
        valor = (ai + 1)* ( pow(10, aj));
        ponerAlFinal(p_pc,(void*)&valor, sizeof(int));

        if(full_board(board))
        {
            return 0;
        }

        return 1;
    }
    // Retorna 0 (Es decir, NO pudo elegir aleatorio (matriz llena) y es EMPATE
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
                sacarUltimoLista(p,&drop,sizeof(int));
            }
        }

    }
    return 0;
}

void finish_match(tPlays *p)
{
    vaciarLista(&p->human);
    vaciarLista(&p->pc);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = 0;

    p->pc_symbol    = 0;
    p->pc_symbol    = 0;
    p->human_symbol = 0;
    p->winner       = 0;

}

int list_score(tSession *s, tPlays *p)
{
    tScore score;

    if (!sacarDeCola(&s->players_queue, &score.player, sizeof(tPlayer)))
        return ERROR;

    // Asigna puntaje
    if (p->winner == DRAW)
    {
        score.player.points += 1;  // Empate: por ejemplo 1 punto
    }
    else if (p->winner == HUMAN_WIN)
    {
        score.player.points += 2;  // Victoria
    }
    else
    {
        score.player.points -= 1;  // Derrota
    }

    // Copia el tablero final
    memcpy(score.board, board, sizeof(score.board));

    score.result = p->winner;

    if (!ponerAlFinal(&s->score_list, &score, sizeof(tScore)))
        return ERROR;

    return OK;
}

void save_game_report_list(tLista *score_list)
{
    int col, fila;
    time_t now;
    char filename[MAX_FILENAME_LENGTH];

    const char *resultadoStr;
    char simbolo;
    tScore *score;

    if (score_list == NULL || *score_list == NULL)
    {
        printf("La lista de puntajes está vacía.\n");
        return;
    }

    now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(filename, sizeof(filename),
             "informe-juego_%04d-%02d-%02d-%02d-%02d-%02d.txt",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error al crear archivo de informe");
        return;
    }

    fprintf(file, "INFORME DE JUEGO (MULTIPLE)\n");
    fprintf(file, "--------------------------\n\n");

    tNodo *actual = *score_list;
    tPlayerSummary summaries[100];  // suponemos como máximo 100 jugadores distintos
    int summary_count = 0;
    int max_points = 0;

    while (actual != NULL)
    {
        score = (tScore *)actual->info;

        fprintf(file, "Jugador: %s\n", score->player.name);
        fprintf(file, "Puntos de la partida: %d\n", score->player.points);

        if (score->result == HUMAN_WIN)
            resultadoStr = "VICTORIA";
        else if (score->result == PC_WIN)
            resultadoStr = "DERROTA";
        else
            resultadoStr = "EMPATE";

        fprintf(file, "Resultado: %s\n", resultadoStr);
        fprintf(file, "ESTADO DEL TABLERO:\n");
        fprintf(file, "-------------\n");

        for (fila = 0; fila < 3; fila++)
        {
            fprintf(file, "|");
            for (col = 0; col < 3; col++)
            {
                if (score->board[fila][col] == XSYM)
                    simbolo = 'X';
                else if (score->board[fila][col] == OSYM)
                    simbolo = 'O';
                else
                    simbolo = ' ';

                fprintf(file, " %c |", simbolo);
            }
            fprintf(file, "\n");
            if (fila < 2)
                fprintf(file, "---+---+---\n");
        }
        fprintf(file, "-------------\n\n");

        find_or_add_player(summaries, &summary_count, score->player.name, score->player.points);

        actual = actual->sig;
    }

    // Determinar puntaje máximo
    for (int i = 0; i < summary_count; i++)
    {
        if (summaries[i].total_points > max_points)
            max_points = summaries[i].total_points;
    }

    // Mostrar ranking
    fprintf(file, "RANKING LOCAL:\n");
    fprintf(file, "--------------\n");
    for (int i = 0; i < summary_count; i++)
    {
        fprintf(file, "%s : %d puntos\n", summaries[i].name, summaries[i].total_points);
    }

    // Mostrar el/los con mayor puntaje
    fprintf(file, "\nJUGADOR(ES) CON MAYOR PUNTAJE:\n");
    fprintf(file, "------------------------------\n");
    for (int i = 0; i < summary_count; i++)
    {
        if (summaries[i].total_points == max_points)
        {
            fprintf(file, "%s con %d puntos\n", summaries[i].name, summaries[i].total_points);
        }
    }

    fclose(file);
    printf("Informe guardado como: %s\n", filename);
}


/*

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

//      Esta función permite chequear el ta te ti mucho mas eficientemente ya que se accede directo a la matriz.
//      Pero se decidio utilizar el método de los magic numbers para aplicar conceptos de lista y buscar una alternativa
//      a los IFs masivos para la IA.
*/


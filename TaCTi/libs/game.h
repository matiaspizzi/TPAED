#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "data.h"
// Estados de resultado de la partida
#define WIN         1
#define DRAW        2
#define LOSE        3
#define HUMAN_WIN   4
#define PC_WIN      5

#define PC_PLAY     0xD5
#define HUMAN_PLAY  6


#define ESYM  0
#define XSYM  1
#define OSYM  2

typedef struct
{
    tLista  human;
    tLista  pc;
    int     curr_symbol;
    int     human_symbol;
    int     pc_symbol;
}tPlays;

typedef struct
{
    tPlayer player;
    int board[3][3];
    int result;
}tScore;


void    init_match(tPlays *p);
int     human_playing(int board[3][3], tPlays *p, int i, int j);
int     full_board(int board[3][3]);
int     traverse_tateti(int board[3][3], tLista *p, int *row, int *col);
int     make_row(int magicNum, int* vecAnswers);
int     randomPosition(int  board[3][3], tLista* p_pc, int symbol);
int     pc_playing(int board[3][3], tPlays *p);
int     check_tateti(tLista* l);

int     save_score(tSession *s, int board[3][3],int res);






#endif // GAME_H_INCLUDED

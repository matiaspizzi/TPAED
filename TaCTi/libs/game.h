#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "data.h"
// Estados de resultado de la partida
#define WIN         1
#define DRAW        0xD1
#define HUMAN_WIN   0xD2
#define PC_WIN      0xD3

#define HUMAN_PLAY  0xD4
#define PC_PLAY     0xD5


#define ESYM  0
#define XSYM  1
#define OSYM  2

void    init_match(tPlays *p);
int     human_playing(int board[3][3], tPlays *p, int i, int j);
int     full_board(int board[3][3]);
int     traverse_tateti(int board[3][3], tLista *p, int *row, int *col);
int     make_row(int magicNum, int* vecAnswers);
int     randomPosition(int  board[3][3], tLista* p_pc, int symbol);
int     pc_playing(int board[3][3], tPlays *p);
int     check_tateti(tLista* l);







#endif // GAME_H_INCLUDED

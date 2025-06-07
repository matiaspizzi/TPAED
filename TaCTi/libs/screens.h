/**
    Esta librería contiene todo lo relacionado a la interfaz gráfica.
**/

#ifndef SCREENS_H_INCLUDED
#define SCREENS_H_INCLUDED

#include "raylib.h"
#include "../parameters.h"
#include "TDA_Lista.h"
#include "cola.h"

#define     MAX_PLAYER_NAME     30

#define     COLOR_TEXT      WHITE
#define     COLOR_BG        DARKGREEN
#define     COLOR_BTN       DARKGRAY
#define     COLOR_BOARD     DARKGRAY
#define     COLOR_X         RED
#define     COLOR_O         BLUE


// Listamos las diferentes pantallas que pueden aparecer.
typedef enum
{
    MENU,
    PLAYERS,
    BOARD,
    RANKING,
    PLAYER_READY,
    EXIT
} screens;


extern const int screenWidth;
extern const int screenHeight;

void    draw_menu               (void);
void    draw_input_player       (void);
void    draw_board              (void);
void    draw_ranking            (void);
void    draw_player_ready       (void);

extern Rectangle btnPlay;
extern Rectangle btnRanking;
extern Rectangle btnExit;
extern Rectangle btnNewPlayer;
extern Rectangle btnStart;
extern Rectangle btnBack;
extern Rectangle btnSurrender;
extern Rectangle txtBox;
extern Rectangle grid[3][3];

#endif // SCREENS_H_INCLUDED

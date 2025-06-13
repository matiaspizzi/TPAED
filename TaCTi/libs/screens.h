/**
    Esta librer�a contiene todo lo relacionado a la interfaz gr�fica.
**/

#ifndef SCREENS_H_INCLUDED
#define SCREENS_H_INCLUDED

#include "raylib.h"
#include "../parameters.h"
#include "TDA_Lista.h"
#include "cola.h"
#include "data.h"

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
    ENTER_PLAYERS,
    BOARD,
    RANKING,
    ROUND,
    PLAYERS_READY,
    EXIT
} screens;


extern const int screenWidth;
extern const int screenHeight;

void    draw_menu               (void);
void    draw_enter_players       (tInput *input);
void    draw_board              (void);
void    draw_ranking            (void);
void    draw_player_ready       (void);
void    draw_round(tLista *players);

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

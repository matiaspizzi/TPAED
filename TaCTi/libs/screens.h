/**
    Esta librer�a contiene todo lo relacionado a la interfaz gr�fica.
**/

#ifndef SCREENS_H_INCLUDED
#define SCREENS_H_INCLUDED

#include "raylib.h"
#include "data.h"


#define     COLOR_TEXT      WHITE
#define     COLOR_BG        DARKGREEN
#define     COLOR_BTN       DARKGRAY
#define     COLOR_BOARD     DARKGRAY
#define     COLOR_X         RED
#define     COLOR_O         BLUE

#define     CELL_SIZE       120

// Listamos las diferentes pantallas que pueden aparecer.
typedef enum
{
    MENU,
    ENTER_PLAYERS,
    ROUND,
    PLAYERS_READY,
    BOARD,
    GAME_OVER,
    END_SAVE,
    RANKING,
    EXIT
} screens;

extern const char *screens_str[];

extern const int screenWidth;
extern const int screenHeight;


void    draw_menu               (void);
void    clear_ranking_cache     (void);
void    draw_enter_players      (tSession *s);
void    draw_board              (tSession *s);
void    draw_ranking            (tSession *s);
void    draw_player_ready       (tSession *s);
void    draw_round              (tSession *s);
void    draw_game_over          ();


extern Rectangle btnPlay;
extern Rectangle btnRanking;
extern Rectangle btnExit;
extern Rectangle btnNewPlayer;
extern Rectangle btnStart;
extern Rectangle btnBack;
extern Rectangle btnNext;
extern Rectangle btnSurrender;
extern Rectangle txtBox;

//  Tablero
extern Rectangle grid[3][3];
extern Rectangle cell;
extern int board[3][3];

#endif // SCREENS_H_INCLUDED

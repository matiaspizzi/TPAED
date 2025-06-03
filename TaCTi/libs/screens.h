/**
    Esta librería contiene todo lo relacionado a la interfaz gráfica.
**/

#ifndef SCREENS_H_INCLUDED
#define SCREENS_H_INCLUDED

#include "raylib.h"
#include "../parameters.h"
#include "TDA_Lista.h"


#define     COLOR_TEXT      WHITE
#define     COLOR_BG        DARKGREEN
#define     COLOR_BTN       DARKGRAY
#define     COLOR_BOARD     DARKGRAY
#define     COLOR_X         RED
#define     COLOR_O         BLUE
// Listamos las diferentes pantallas que pueden aparecer.
typedef enum
{
    MENU = 0xA1,
    PLAYERS = 0xA2,
    BOARD = 0xA3,
    RANKING = 0xA4,
    PLAYER_READY = 0xA5,
    EXIT = 0xE1
} screens;


extern const int screenWidth;
extern const int screenHeight;

int draw_menu(Vector2 mouse);
int draw_input_player(Vector2 mouse, tLista *players);
int draw_board(Vector2 mouse);
int draw_ranking(Vector2 mouse);
int draw_player_ready(Vector2 mouse, const char* playerName);

#endif // SCREENS_H_INCLUDED

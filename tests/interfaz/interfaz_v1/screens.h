#ifndef SCREENS_H_INCLUDED
#define SCREENS_H_INCLUDED

#include "raylib.h"

#define     COLOR_TEXT      WHITE
#define     COLOR_BG        DARKGREEN
#define     COLOR_BTN       DARKGRAY

// Listamos las diferentes pantallas que pueden aparecer.
typedef enum
{
    MENU = 0xA1,
    PLAYERS = 0xA2,
    EXIT = 0xE1
} screens;


extern const int screenWidth;
extern const int screenHeight;

int draw_menu(Vector2 mouse);

#endif // SCREENS_H_INCLUDED

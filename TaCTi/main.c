#include <stdio.h>
#include <stdlib.h>
#include "libs/screens.h"
#include "parameters.h"
#include "libs/TDA_Lista.h"


int main()
{
    InitWindow(screenWidth, screenHeight, "TaCTi");
    SetTargetFPS(30);
    //tPlayer player;           //  Estructura del jugador
    tLista inputPlayers;        //  Lista para almacenar los jugadores que se ingresan.
    crearLista(&inputPlayers);

    screens currentScreen = MENU;
    while (!WindowShouldClose() && currentScreen != EXIT)
    {
        BeginDrawing();
        ClearBackground(COLOR_BG);
        Vector2 mouse = GetMousePosition();
        switch(currentScreen)
        {
            case MENU:
            {
                currentScreen = draw_menu(mouse);
                break;
            }
            case PLAYERS:
            {
                currentScreen = draw_input_player(mouse, &inputPlayers);
                break;
            }
            case PLAYER_READY:
            {
                currentScreen = draw_player_ready(mouse, "&player_name");
                break;
            }
            case BOARD:
            {
                currentScreen = draw_board(mouse);
                break;
            }
            case RANKING:
            {
                currentScreen = draw_ranking(mouse);
                break;
            }
            case EXIT:
            {
                break;
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

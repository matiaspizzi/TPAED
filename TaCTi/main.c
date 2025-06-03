#include <stdio.h>
#include <stdlib.h>

#include "parameters.h"

#include "libs/screens.h"
#include "libs/TDA_Lista.h"
#include "libs/data.h"
#include "libs/cola.h"


int main()
{
    InitWindow(screenWidth, screenHeight, "TaCTi");
    SetTargetFPS(30);
    screens currentScreen = MENU;
    tLista inputPlayers;
    crearLista(&inputPlayers);
    tCola gameTurn;
    crearCola(&gameTurn);
    char buffer[MAX_BUFF_SIZE];

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
                currentScreen = draw_input_player(mouse, &inputPlayers, &gameTurn);
                break;
            }
            case PLAYER_READY:
            {
                currentScreen = draw_player_ready(mouse);
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

    printf("Jugadores ingresados: \n");
    recorrerLista(&inputPlayers,MAX_BUFF_SIZE,printString);
    /*while(!colaVacia(&gameTurn))
    {
        sacarDeCola(&gameTurn, buffer, MAX_BUFF_SIZE);
        printf("%s\n", buffer);
    }*/
    return 0;
}

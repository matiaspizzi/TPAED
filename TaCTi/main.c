#include <stdio.h>
#include <stdlib.h>

#include "parameters.h"

#include "libs/screens.h"
#include "libs/interactions.h"

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

    while (!WindowShouldClose() && currentScreen != EXIT)
    {
        BeginDrawing();
        ClearBackground(COLOR_BG);
        switch(currentScreen)
        {
            case MENU:
            {
                draw_menu();
                currentScreen = update_menu();
                break;
            }
            case PLAYERS:
            {
                draw_input_player();
                currentScreen = update_input_player();
                break;
            }
            case PLAYER_READY:
            {
                draw_player_ready();

                break;
            }
            case BOARD:
            {
                draw_board();
                break;
            }
            case RANKING:
            {
                draw_ranking();
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

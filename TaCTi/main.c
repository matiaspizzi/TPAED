#include <stdio.h>
#include <stdlib.h>

#include "parameters.h"

#include "libs/screens.h"
#include "libs/interactions.h"

#include "libs/data.h"

#include "libs/TDA_Lista.h"
#include "libs/cola.h"


int main()
{
    InitWindow(screenWidth, screenHeight, "TaCTi");
    SetTargetFPS(30);

    screens currentScreen = MENU;
    screens prevScreen = MENU;

    tSession session;
    init_session(&session);

    tPlays plays;

    while (!WindowShouldClose() && currentScreen != EXIT)
    {

        if (prevScreen != currentScreen && currentScreen == RANKING) {
            clear_ranking_cache();
        }

        prevScreen = currentScreen;

        BeginDrawing();
        ClearBackground(COLOR_BG);
        switch(currentScreen)
        {
            case MENU:
            {
                //  Se despliega el menu.
                draw_menu();
                currentScreen = update_menu();

                break;
            }
            case ENTER_PLAYERS:
            {
                //  Se reciben los nombres de jugadores y enlistan.
                draw_enter_players(&session);
                currentScreen = update_enter_players(&session);

                break;
            }
            case ROUND:
            {
                //  Se otorgan los turnos para cada jugador.
                draw_round(&session);
                currentScreen = update_round(&session);

                break;
            }
            case PLAYERS_READY:
            {
                draw_player_ready(&session);
                currentScreen = update_player_ready(&session, &plays);
                break;
            }
            case BOARD:
            {
                draw_board(&session);
                currentScreen = update_board(&session,&plays);
                break;
            }
            case GAME_OVER:
            {
                draw_game_over(&session);
                currentScreen = update_game_over(&session,&plays);
                break;

            }
            case RANKING:
            {
                draw_ranking(&session);
                currentScreen = update_ranking(&session);

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
    recorrerMostrarLista(&session.players_list,MAX_BUFF_SIZE,printString);
    /*while(!colaVacia(&gameTurn))
    {
        sacarDeCola(&gameTurn, buffer, MAX_BUFF_SIZE);
        printf("%s\n", buffer);
    }*/
    return 0;
}

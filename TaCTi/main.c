#include <stdio.h>
#include <stdlib.h>
#include "libs/screens.h"
#include "libs/interactions.h"
#include "libs/data.h"
#include "libs/requests.h"

int main()
{
    InitWindow(screenWidth, screenHeight, "TaCTi");
    SetTargetFPS(30);       //  Minimo 20 FPS, menos no. 30 cumple.

    screens currentScreen = MENU;
    screens prevScreen = MENU;

    tSession session;
    init_session(&session);

    tPlays plays;

     if (!load_config()) {
        return 1;
    }

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
                //  Se prepara al jugador para comenzar o rendirse.
                draw_player_ready(&session);
                currentScreen = update_player_ready(&session, &plays);
                break;
            }
            case BOARD:
            {
                //  Se efectua el juego.
                draw_board(&session);
                currentScreen = update_board(&session,&plays);
                break;
            }
            case GAME_OVER:
            {
                //  Se termina el juego.
                draw_game_over(&plays);
                currentScreen = update_game_over(&session,&plays);
                break;

            }
            case END_SAVE:
            {
                //  Guarda los datos en la API y en local.
                currentScreen = update_end_save(&session,&plays);
                break;
            }
            case RANKING:
            {
                //  Muestra el TOP 10 desde la API.
                draw_ranking(&session);
                currentScreen = update_ranking(&session);

                break;
            }
            case EXIT:
            {
                //  Fin.
                break;
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

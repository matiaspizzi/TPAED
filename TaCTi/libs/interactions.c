#include <stdio.h>
#include "raylib.h"
#include "screens.h"
#include "interactions.h"
#include "data.h"
#include "game.h"
float errorTimer;
int showError = 0;

int update_menu()
{
    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, btnExit) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("--> SALIR\n");
        return EXIT;
    }
    if (CheckCollisionPointRec(mouse, btnPlay) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("--> JUGAR\n");
        return ENTER_PLAYERS;
    }
    if (CheckCollisionPointRec(mouse, btnRanking) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        //printf("--> VER RANKING\n");
        //return RANKING;
    }
    return MENU;
}

int update_enter_players(tSession *s)
{
    char key;
    Vector2 mouse = GetMousePosition();

    //  Recibo el nombre de jugador en txt box.
    key = GetCharPressed();
    while (key > 0)
    {
        if (isVALIDCHAR(key) && s->input.keyCount < MAX_BUFF_SIZE - 1)
        {
            s->input.name[s->input.keyCount] = key;
            s->input.keyCount++;
            s->input.name[s->input.keyCount] = '\0';
        }
        key = GetCharPressed();
    }

    //  Si se presiona backspace (borrar) elimina el ultimo caracter.
    if (IsKeyPressed(KEY_BACKSPACE) && s->input.keyCount > 0)
    {
        s->input.keyCount--;
        s->input.name[s->input.keyCount] = '\0';
    }

    //  Boton atras --> Deshace lo ingresado y vuelve al menú.
    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("-> ATRAS\n");
        drop_session(s);
        return MENU;
    }

    //  Boton otro jugador --> Agrega al jugador a la lista y permite seguir ingresando más.
    if (CheckCollisionPointRec(mouse, btnNewPlayer) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (list_player(s))
        {
            reset_input(&s->input);
            s->qtyPlayers ++;
        }
        else
        {
            errorTimer = 2.0f;
            showError = 1;
        }
        return ENTER_PLAYERS;
    }
    //  Boton comenzar --> Agrega al jugador a la lista y  procede a otorgar los turnos.
    if (CheckCollisionPointRec(mouse, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
            if (s->input.keyCount > 0 && s->input.name[0] != '\0')
            {
                if (list_player(s))
                    s->qtyPlayers++;
                else
                {
                    errorTimer = 2.0f;
                    showError = 1;
                }
            }
            else if(s->qtyPlayers < 1)
            {
                errorTimer = 2.0f;
                showError = 1;
                return ENTER_PLAYERS;
            }
            reset_input(&s->input);
            queue_player(s);
            return ROUND;
    }
    if (showError)
    {
        DrawText("Error",
                 screenWidth/2 - MeasureText("Error", 30)/2, 50, 30, RED);
        errorTimer -= GetFrameTime();
        if (errorTimer <= 0)
        {
            showError = 0;
        }
    }
    return ENTER_PLAYERS;
}

int update_round(tSession *s)
{
    Vector2 mouse = GetMousePosition();
    //  Boton atras.
    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return ENTER_PLAYERS;
    }
    if(CheckCollisionPointRec(mouse, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        vaciarLista(&s->players_list);
        return PLAYERS_READY;
    }
    return ROUND;
}

int update_ranking(tSession *s)
{
    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return MENU;
    }

    return RANKING;
}

int update_player_ready(tSession *s, tPlays *p)
{
   Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, btnSurrender) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        drop_session(s);
        return MENU;
    }

    if (CheckCollisionPointRec(mouse, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        init_match(p);
        return BOARD;
    }
    return PLAYERS_READY;
}


int update_board(tSession *s, tPlays *p)
{
    Vector2 mouse = GetMousePosition();

    int res = 0;
    int row,col;
    // TURNO HUMANO
    if (p->curr_symbol == p->human_symbol)
    {
        for (row = 0; row < 3; row++)
        {
            for (col = 0; col < 3; col++)
            {
                Rectangle cell = grid[row][col];
                if (CheckCollisionPointRec(mouse, cell) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    if (board[row][col] == 0)
                    {
                        res = human_playing(board, p,row,col);
                        switch(res)
                        {
                            case DRAW:
                            {
                                printf("Empate\n");
                                return GAME_OVER;
                                break;
                            }
                            case HUMAN_WIN:
                            {
                                printf("Gana usuario\n");
                                return GAME_OVER;
                            }
                            case PC_PLAY:
                            {
                                p->curr_symbol = p->pc_symbol;
                                break;
                            }
                        }
                        return BOARD;
                    }
                }
            }
        }
    }
    // TURNO PC
    else
    {
        // PC juega (intenta ganar / bloquear / jugada estratégica)
        res = pc_playing(board,p);
        //board[row][col] = p->pc_symbol;
        switch(res)
        {
            case DRAW:
            {
                printf("Empate\n");
                return GAME_OVER;
                break;
            }
            case PC_WIN:
            {
                printf("Gana PC\n");
                return GAME_OVER;
            }
            case HUMAN_PLAY:
            {
                p->curr_symbol = p->human_symbol;
                break;
            }
        }
    }
    return BOARD;
}

int update_game_over()
{
   /* Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, btnNext) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("--> GUARDAR\n");
        return MENU;
    }*/
    return GAME_OVER;
}




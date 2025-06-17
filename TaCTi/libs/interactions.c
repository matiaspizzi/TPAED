#include <stdio.h>
#include "raylib.h"
#include "screens.h"
#include "interactions.h"
#include "data.h"

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
        printf("--> VER RANKING\n");
        return RANKING;
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
        reset_input(&s->input);
        vaciarLista(&s->players_list); ///-----< cambiar
        return MENU;
    }

    //  Boton otro jugador --> Agrega al jugador a la lista y permite seguir ingresando más.
    if (CheckCollisionPointRec(mouse, btnNewPlayer) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (s->input.keyCount > 3)
        {
            list_player(s);
            reset_input(&s->input);
            s->qtyPlayers++;
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
        if (s->input.keyCount > 3 || !listaVacia(&s->players_list))
        {
            list_player(s);
            reset_input(&s->input);
            s->qtyPlayers++;
            if(s->qtyPlayers == 1)
                return BOARD;
            return ROUND;
        }
        else
        {
            errorTimer = 2.0f;
            showError = 1;
        }
    }


    if (showError)
    {
        DrawText("Ingrese minimo 1 jugador",
                 screenWidth/2 - MeasureText("Ingrese minimo 1 jugador", 30)/2, 50, 30, RED);
        errorTimer -= GetFrameTime();
        if (errorTimer <= 0)
        {
            showError = 0;
        }
    }
    return ENTER_PLAYERS;
}


int update_round()
{
    Vector2 mouse = GetMousePosition();
    //  Boton atras.
    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return MENU;
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


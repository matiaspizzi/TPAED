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

int update_enter_players(tInput *input, tLista *players)
{
    char key;
    Vector2 mouse = GetMousePosition();
    key = GetCharPressed();
    while (key > 0)
    {
        if (isVALIDCHAR(key) && input->keyCount < MAX_BUFF_SIZE - 1)
        {
            input->name[input->keyCount] = key;
            input->keyCount++;
            input->name[input->keyCount] = '\0';
        }
        key = GetCharPressed();
    }

    //  Si se presiona backspace (borrar) elimina el ultimo caracter.
    if (IsKeyPressed(KEY_BACKSPACE) && input->keyCount > 0)
    {
        input->keyCount--;
        input->name[input->keyCount] = '\0';
    }

    //  Boton atras --> Deshace lo ingresado y vuelve al menú.
    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        clear_input(input);
        vaciarLista(players); ///-----< cambiar
        return MENU;
    }

    //  Boton otro jugador --> Agrega al jugador a la lista y permite seguir ingresando más.
    if (CheckCollisionPointRec(mouse, btnNewPlayer) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (input->keyCount > 0)
        {
            list_player(players,input);
            reset_input(input);
        }
    }

    //  Boton comenzar --> Agrega al jugador a la lista y  procede a otorgar los turnos.
    if (CheckCollisionPointRec(mouse, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (input->keyCount > 0)
        {
            list_player(players,input);
            reset_input(input);
            return ROUND;
        }
        else
        {
            errorTimer = 2.0f;
            showError = 1;
        }
    }

    // --- Mostrar error (esto debe ir afuera de los ifs anteriores) ---
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

int update_ranking()
{
    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return MENU;
    }

    return RANKING;
}


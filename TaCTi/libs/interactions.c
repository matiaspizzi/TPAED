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
        return EXIT;
    }
    if (CheckCollisionPointRec(mouse, btnPlay) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return PLAYERS;
    }
    if (CheckCollisionPointRec(mouse, btnRanking) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return RANKING;
    }
    return MENU;
}

int update_input_player()
{
    char key;
    Vector2 mouse = GetMousePosition();
    key = GetCharPressed();
    while (key > 0)
    {
        if (isVALIDCHAR(key) && input.keyCount < MAX_BUFF_SIZE - 1)
        {
            input.name[input.keyCount] = key;
            input.keyCount++;
            input.name[input.keyCount] = '\0';
        }
        key = GetCharPressed();
    }

    //  Si se presiona backspace (borrar) elimina el ultimo caracter.
    if (IsKeyPressed(KEY_BACKSPACE) && input.keyCount > 0)
    {
        input.keyCount--;
        input.name[input.keyCount] = '\0';
    }
    /// ----------------------------------------------------------------------------------------------------
    /// Interacciones   ------------------------------------------------------------------------------------
    //  Boton atras.
    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        input.name[0] = '\0';
        input.keyCount = 0;
        return MENU;
    }

    //  Boton otro jugador.
    if (CheckCollisionPointRec(mouse, btnNewPlayer) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (input.keyCount > 0)
        {
            //ponerAlFinal(players, input.name, input.keyCount);
            input.cantPlayers++;
            input.name[0] = '\0';
            input.keyCount = 0;
        }
    }

    //  Boton comenzar.
    if (CheckCollisionPointRec(mouse, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (input.keyCount > 0)
        {
            input.cantPlayers++;
            input.name[0] = '\0';
            input.keyCount = 0;

            //ponerAlFinal(players, input, keyCount);
            /*desordenarLista(players);
            while(!listaVacia(players))
            {
                sacarAlFinal(players,&name,MAX_BUFF_SIZE);
                ponerEnCola(gameTurn,&name,strlen(name));
            }*/
        }
        if (input.cantPlayers >= 1)
            return PLAYER_READY;
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
    return PLAYERS;
}
/*




    /// ----------------------------------------------------------------------------------------------------
*/

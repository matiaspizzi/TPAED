#include "screens.h"
#include "raylib.h"


const int screenWidth = 800;
const int screenHeight = 600;

//  posicion en x, posicion en y, ancho, alto
    Rectangle btnPlay       = { screenWidth/2.0f - 150, 200, 300, 50 };
    Rectangle btnRanking    = { screenWidth/2.0f - 150, 280, 300, 50 };
    Rectangle btnExit       = { screenWidth/2.0f - 150, 360, 300, 50 };


int draw_menu(Vector2 mouse)
{
    DrawText("Menu Principal", screenWidth/2 - MeasureText("Menu Principal", 30)/2, 120, 30, COLOR_TEXT);

    DrawRectangleRec(btnPlay, COLOR_BTN);
    DrawText("JUGAR",
        btnPlay.x + (btnPlay.width - MeasureText("JUGAR", 20)) / 2,
        btnPlay.y + (btnPlay.height - 20) / 2,
        20, COLOR_TEXT);

    DrawRectangleRec(btnRanking, COLOR_BTN);
    DrawText("RANKING",
        btnRanking.x + (btnRanking.width - MeasureText("RANKING", 20)) / 2,
        btnRanking.y + (btnRanking.height - 20) / 2,
        20, COLOR_TEXT);

    DrawRectangleRec(btnExit, COLOR_BTN);
    DrawText("SALIR",
        btnExit.x + (btnExit.width - MeasureText("SALIR", 20)) / 2,
        btnExit.y + (btnExit.height - 20) / 2,
        20, COLOR_TEXT);


    if (CheckCollisionPointRec(mouse, btnExit) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return EXIT;
    }
    if (CheckCollisionPointRec(mouse, btnPlay) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return PLAYERS;
    }
    return MENU;
}


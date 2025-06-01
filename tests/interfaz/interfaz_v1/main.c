#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"
#include "screens.h"

int main(void)
{
    InitWindow(screenWidth, screenHeight, "TaCTi");

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


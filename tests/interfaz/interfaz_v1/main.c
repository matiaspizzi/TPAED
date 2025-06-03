#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"
#include "screens.h"

#define MAX_BUFF_SIZE 50


int main(void)
{
    InitWindow(screenWidth, screenHeight, "TaCTi");

    screens currentScreen = MENU;
    char buff_txt[MAX_BUFF_SIZE];

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
                currentScreen = draw_input_player(mouse);
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


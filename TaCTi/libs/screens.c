#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "screens.h"
#include "data.h"

#include "game.h"
#include "requests.h"

const int screenWidth = 800;
const int screenHeight = 600;

const char *screens_str[] = {"MENU","ENTER_PLAYERS","BOARD","RANKING","ROUND","PLAYERS_READY","EXIT"};

int board[3][3] = {0};

//  Posicion en x, posicion en y, ancho, alto
//  Botones, caja de texto y tablero. Se hacen globales ya que se reutilizan en diferentes funciones y pantallas.
Rectangle btnPlay       =   { screenWidth/2.0f - 150, 200, 300, 50 };
Rectangle btnRanking    =   { screenWidth/2.0f - 150, 280, 300, 50 };
Rectangle btnExit       =   { screenWidth/2.0f - 150, 360, 300, 50 };
Rectangle btnNewPlayer  =   { screenWidth/2.0f - 150, 280, 300, 50 };
Rectangle btnStart      =   { screenWidth/2.0f - 150, 360, 300, 50 };
Rectangle btnBack       =   { screenWidth/2.0f - 150, 440, 300, 50 };
Rectangle btnNext       =   { screenWidth/2.0f - 150, 440, 300, 50 };
Rectangle btnSurrender  =   { screenWidth/2.0f - 150, 440, 300, 50 };
Rectangle txtBox        =   { screenWidth/2.0f - 200, 200, 400, 50 };
Rectangle grid[3][3];



static          tPlayer *cached_players = NULL;
static int      cached_players_count = 0;
static double   last_fetch_time = 0;
const double    REFRESH_INTERVAL = 30.0;

void clear_ranking_cache()
{
    if (cached_players != NULL) {
        free(cached_players);
        cached_players = NULL;
    }
    cached_players_count = 0;
}

int compare_players(const void *a, const void *b) {
    const tPlayer *playerA = (const tPlayer *)a;
    const tPlayer *playerB = (const tPlayer *)b;
    return playerB->points - playerA->points;
}

void sort_ranking(tPlayer *players, int count) {
    if (players == NULL || count <= 0) return;
    qsort(players, count, sizeof(tPlayer), compare_players);
}

void draw_menu(void)
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
}

void draw_enter_players(tSession *s)
{
    DrawText("Ingrese nombre", screenWidth/2 - MeasureText("Ingrese nombre", 30)/2, 120, 30, COLOR_TEXT);

    DrawRectangleRec(txtBox, GRAY);
    DrawRectangleLinesEx(txtBox, 2, DARKGRAY);
    DrawText(s->input.name, txtBox.x + 10, txtBox.y + 15, 20, BLACK);

    DrawRectangleRec(btnNewPlayer, COLOR_BTN);
    DrawText("OTRO JUGADOR",
        btnNewPlayer.x + (btnNewPlayer.width - MeasureText("OTRO JUGADOR", 20)) / 2,
        btnNewPlayer.y + (btnNewPlayer.height - 20) / 2,
        20, COLOR_TEXT);

    DrawRectangleRec(btnStart, COLOR_BTN);
    DrawText("EMPEZAR",
        btnStart.x + (btnStart.width - MeasureText("EMPEZAR", 20)) / 2,
        btnStart.y + (btnStart.height - 20) / 2,
        20, COLOR_TEXT);

    DrawRectangleRec(btnBack, COLOR_BTN);
    DrawText("ATRAS",
        btnBack.x + (btnBack.width - MeasureText("ATRAS", 20)) / 2,
        btnBack.y + (btnBack.height - 20) / 2,
        20, COLOR_TEXT);
}

void draw_board(tSession *s)
{
    //  Se ubica el tablero en el centro de la ventana.
    const int offsetX = screenWidth/2 - (CELL_SIZE * 3) / 2;
    const int offsetY = screenHeight/2 - (CELL_SIZE * 3) / 2;
    int fontSize = 80;
    int textWidth = MeasureText("X", fontSize);
    int textHeight = fontSize; // Raylib no tiene MeasureTextHeight, pero se asume aprox = fontSize

    char buffer[MAX_BUFF_SIZE];
    tPlayer p;
    get_player(&p, s);
    sprintf(buffer, "Turno de: %s", p.name);

    /// Visuales   -----------------------------------------------------------------------------------------
    DrawText(buffer, screenWidth/2 - MeasureText(buffer, 30)/2, 50, 30, COLOR_TEXT);
    // Dibujar las celdas y guardar las posiciones
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            Rectangle cell = { offsetX + col * CELL_SIZE, offsetY + row * CELL_SIZE, CELL_SIZE, CELL_SIZE };
            grid[row][col] = cell;

            DrawRectangleRec(cell, COLOR_BOARD);
            DrawRectangleLinesEx(cell, 2, COLOR_TEXT);

            // Dibujar X / O
            if (board[row][col] == XSYM)
                DrawText("X", cell.x + (CELL_SIZE - textWidth) / 2,
                    cell.y + (CELL_SIZE - textHeight) / 2,
                    fontSize, COLOR_X);
            else if (board[row][col] == OSYM)
                DrawText("O", cell.x + (CELL_SIZE - textWidth) / 2,
                    cell.y + (CELL_SIZE - textHeight) / 2,
                    fontSize, COLOR_O);
        }
    }
}

void draw_ranking(tSession *s)
{
    int i = 0;
    char buffer[MAX_BUFF_SIZE];
    double current_time = GetTime();

    DrawText("Ranking", screenWidth/2 - MeasureText("Ranking", 30)/2, 50, 30, COLOR_TEXT);

    // Solo hace la petición si:
    // - No hay datos en caché O
    // - Pasó el intervalo de refresco
    if (cached_players == NULL || (current_time - last_fetch_time) > REFRESH_INTERVAL) {
        if (cached_players != NULL) {
            free(cached_players);
            cached_players = NULL;
        }

        cached_players_count = get_players(&cached_players);
        sort_ranking(cached_players, cached_players_count);
        last_fetch_time = current_time;
    }

    if (cached_players != NULL && cached_players_count > 0) {
        while(i<cached_players_count && i<10){
            snprintf(buffer, sizeof(buffer), "%d - %s: %i points",
                        i+1, cached_players[i].name, cached_players[i].points);

            if (100 + i*35 < screenHeight) {
                DrawText(buffer, 100, 100 + i*35, 30, COLOR_TEXT);
            }
            i++;
        }
    }
    if (cached_players_count == 0) DrawText("Sin datos", 100, 100 + i*35, 30, COLOR_TEXT);


    DrawRectangleRec(btnBack, COLOR_BTN);
    DrawText("ATRAS",
        btnBack.x + (btnBack.width - MeasureText("ATRAS", 20))/2,
        btnBack.y + (btnBack.height - 20)/2,
        20, COLOR_TEXT);
}

void draw_player_ready(tSession *s)
{

    char buffer[MAX_BUFF_SIZE];
    tPlayer p;

    if(get_player(&p, s))
    {
        sprintf(buffer, "%s estas listo?", p.name);

        DrawText(buffer, screenWidth / 2 - MeasureText(buffer, 30) / 2, screenHeight / 2 - 50, 30, COLOR_TEXT);

        DrawRectangleRec(btnStart, COLOR_BTN);
        DrawText("COMENZAR",
            btnStart.x + (btnStart.width - MeasureText("COMENZAR", 20)) / 2,
            btnStart.y + (btnStart.height - 20) / 2,
            20, COLOR_TEXT);

        DrawRectangleRec(btnSurrender, COLOR_BTN);
        DrawText("RENDIRSE",
            btnSurrender.x + (btnSurrender.width - MeasureText("RENDIRSE", 20)) / 2,
            btnSurrender.y + (btnSurrender.height - 20) / 2,
            20, COLOR_TEXT);
    }
}

void draw_round(tSession *s)
{
    int i = 0;
    char name[MAX_BUFF_SIZE];
    char buffer[MAX_BUFF_SIZE + 6]; // Mas espacio para evitar desbordamiento

    DrawText("Turnos asignados", screenWidth / 2 - MeasureText("Turnos asignados", 30) / 2, 50, 30, COLOR_TEXT);

    for(i = 0 ; i < s->qtyPlayers; i++)
    {
        verEnPosicion(&s->players_list,i,&name,MAX_BUFF_SIZE);
        sprintf(buffer, "%d - %s", i+1, name);
        DrawText(buffer, 100, 100 + i * 35, 30, COLOR_TEXT);
    }

    DrawRectangleRec(btnStart, COLOR_BTN);
    DrawText("EMPEZAR",
        btnStart.x + (btnStart.width - MeasureText("EMPEZAR", 20)) / 2,
        btnStart.y + (btnStart.height - 20) / 2,
        20, COLOR_TEXT);

    DrawRectangleRec(btnBack, COLOR_BTN);
    DrawText("ATRAS",
        btnBack.x + (btnBack.width - MeasureText("ATRAS", 20)) / 2,
        btnBack.y + (btnBack.height - 20) / 2,
        20, COLOR_TEXT);
}

void draw_game_over(tScore *sc)
{
    const int offsetX = screenWidth / 2 - (CELL_SIZE * 3) / 2;
    const int offsetY = screenHeight / 2 - (CELL_SIZE * 3) / 2;
    int fontSize = 80;
    int textWidth = MeasureText("X", fontSize);
    int textHeight = fontSize;
    char buffer[MAX_BUFF_SIZE];
    if(sc->result == DRAW)
        memcpy(buffer, "Empate",strlen("Empate"));
    else if(sc->result == HUMAN_WIN)
        sprintf(buffer,"Gano: %s",sc->player.name);
    else
        memcpy(buffer, "Gano PC",strlen("Gano PC"));

    DrawText(buffer, screenWidth/2 - MeasureText(buffer, 30)/2, 30, 30, COLOR_TEXT);
    // Tablero y piezas
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            Rectangle cell = { offsetX + col * CELL_SIZE, offsetY + row * CELL_SIZE, CELL_SIZE, CELL_SIZE };
            grid[row][col] = cell;

            DrawRectangleRec(cell, COLOR_BOARD);
            DrawRectangleLinesEx(cell, 2, COLOR_TEXT);

            if (board[row][col] == XSYM)
                DrawText("X", cell.x + (CELL_SIZE - textWidth) / 2,
                              cell.y + (CELL_SIZE - textHeight) / 2,
                              fontSize, COLOR_X);
            else if (board[row][col] == OSYM)
                DrawText("O", cell.x + (CELL_SIZE - textWidth) / 2,
                              cell.y + (CELL_SIZE - textHeight) / 2,
                              fontSize, COLOR_O);
        }
    }

    // Botón SIGUIENTE
    btnNext = (Rectangle){ screenWidth / 2 - 100, offsetY + CELL_SIZE * 3 + 40, 200, 50 };
    DrawRectangleRec(btnNext, COLOR_BTN);
    DrawText("SIGUIENTE",
             btnNext.x + (btnNext.width - MeasureText("SIGUIENTE", 20)) / 2,
             btnNext.y + (btnNext.height - 20) / 2,
             20, COLOR_TEXT);
}



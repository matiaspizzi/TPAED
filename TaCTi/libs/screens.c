#include "screens.h"

const int screenWidth = 800;
const int screenHeight = 600;

//  posicion en x, posicion en y, ancho, alto
//  Botones, caja de texto y tablero. Es necesario que sean globales. Se reutilizan en diferentes pantallas.
Rectangle btnPlay       =   { screenWidth/2.0f - 150, 200, 300, 50 };
Rectangle btnRanking    =   { screenWidth/2.0f - 150, 280, 300, 50 };
Rectangle btnExit       =   { screenWidth/2.0f - 150, 360, 300, 50 };
Rectangle btnNewPlayer  =   { screenWidth/2.0f - 150, 280, 300, 50 };
Rectangle btnStart      =   { screenWidth/2.0f - 150, 360, 300, 50 };
Rectangle btnBack       =   { screenWidth/2.0f - 150, 440, 300, 50 };
Rectangle btnSurrender  =   { screenWidth/2.0f - 150, 440, 300, 50 };
Rectangle txtBox        =   { screenWidth/2.0f - 200, 200, 400, 50 };
Rectangle grid[3][3];


char input[MAX_BUFF_SIZE];
int keyCount = 0;
int cantPlayers = 0;
int currentPlayer = 1; // 1 = X, 2 = O

static tPlayer *cached_players = NULL;
static int cached_players_count = 0;
static double last_fetch_time = 0;
const double REFRESH_INTERVAL = 30.0;


int draw_menu(Vector2 mouse)
{
    /// Visuales   -----------------------------------------------------------------------------------------
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
    /// ----------------------------------------------------------------------------------------------------

    /// Interacciones   ------------------------------------------------------------------------------------
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
    /// ----------------------------------------------------------------------------------------------------

    return MENU;
}

int draw_input_player(Vector2 mouse, tLista *players)
{
    char key;
    static int showError = 0;
    static float errorTimer;

    /// Visuales   -----------------------------------------------------------------------------------------
    DrawText("Ingrese nombre", screenWidth/2 - MeasureText("Ingrese nombre", 30)/2, 120, 30, COLOR_TEXT);
    DrawRectangleRec(txtBox, GRAY);

    DrawRectangleLinesEx(txtBox, 2, DARKGRAY);
    DrawText(input, txtBox.x + 10, txtBox.y + 15, 20, BLACK);

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
    /// ----------------------------------------------------------------------------------------------------

    /// Lectura por teclado --------------------------------------------------------------------------------
    key = GetCharPressed();
    while (key > 0)
    {
        if (isVALIDCHAR(key) && keyCount < MAX_BUFF_SIZE - 1)
        {
            input[keyCount] = key;
            keyCount++;
            input[keyCount] = '\0';
        }
        key = GetCharPressed();
    }

    //  Si se presiona backspace (borrar) elimina el ultimo caracter.
    if (IsKeyPressed(KEY_BACKSPACE) && keyCount > 0)
    {
        keyCount--;
        input[keyCount] = '\0';
    }
    /// ----------------------------------------------------------------------------------------------------


    /// Interacciones   ------------------------------------------------------------------------------------
    //  Boton atras.
    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        input[0] = '\0';
        keyCount = 0;
        return MENU;
    }

    //  Boton otro jugador.
    if (CheckCollisionPointRec(mouse, btnNewPlayer) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (keyCount > 0)
        {
            ponerAlFinal(players,input,keyCount);  // Tu función para agregar jugador
            cantPlayers++;
            input[0] = '\0';
            keyCount = 0;
        }
    }

    //  Boton comenzar.
    if (CheckCollisionPointRec(mouse, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (keyCount > 0)
        {
            ponerAlFinal(players, input, keyCount);
            cantPlayers++;
            input[0] = '\0';
            keyCount = 0;
        }

        if (cantPlayers >= 1)
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

    /// ----------------------------------------------------------------------------------------------------

    return PLAYERS;
}

int draw_board(Vector2 mouse)
{
    //  Se ubica el tablero en el centro de la ventana.
    const int offsetX = screenWidth/2 - (CELL_SIZE * 3) / 2;
    const int offsetY = screenHeight/2 - (CELL_SIZE * 3) / 2;
    int fontSize = 80;
    int textWidth = MeasureText("X", fontSize);
    int textHeight = fontSize; // Raylib no tiene MeasureTextHeight, pero se asume aprox = fontSize

    //aca falta buffer y sprintf para el nombre del jugador

    /// Visuales   -----------------------------------------------------------------------------------------
    DrawText("Turno de: &player_name", screenWidth/2 - MeasureText("Turno de: &player_name", 30)/2, 50, 30, COLOR_TEXT);
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
            if (board[row][col] == 1)
                DrawText("X", cell.x + (CELL_SIZE - textWidth) / 2,
                    cell.y + (CELL_SIZE - textHeight) / 2,
                    fontSize, COLOR_X);
            else if (board[row][col] == 2)
                DrawText("O", cell.x + (CELL_SIZE - textWidth) / 2,
                    cell.y + (CELL_SIZE - textHeight) / 2,
                    fontSize, COLOR_O);

            // Click en celda vacía
            if (CheckCollisionPointRec(mouse, cell) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (board[row][col] == 0)
                {
                    board[row][col] = currentPlayer;
                    currentPlayer = (currentPlayer == 1) ? 2 : 1;
                }
            }
        }
    }

    return BOARD;
}

int draw_ranking(Vector2 mouse) {
    int i = 0;
    char buffer[64];
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


    DrawRectangleRec(btnBack, COLOR_BTN);
    DrawText("ATRAS",
        btnBack.x + (btnBack.width - MeasureText("ATRAS", 20))/2,
        btnBack.y + (btnBack.height - 20)/2,
        20, COLOR_TEXT);

    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return MENU;
    }

    return RANKING;
}

void clear_ranking_cache() {
    if (cached_players != NULL) {
        free(cached_players);
        cached_players = NULL;
    }
    cached_players_count = 0;
}

int draw_player_ready(Vector2 mouse, const char* playerName)
{

    char buffer[64];
    sprintf(buffer, "%s estas listo?", playerName);

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

    if (CheckCollisionPointRec(mouse, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return BOARD;
    }

    if (CheckCollisionPointRec(mouse, btnSurrender) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return PLAYER_READY;
    }

    return PLAYER_READY;
}


#include <stdio.h>
#include "raylib.h"
#include "TDA_Lista.h"
#include "screens.h"
#include "interactions.h"
#include "data.h"
#include "game.h"
#include "requests.h"

float errorTimer;
int showError = 0;

int update_menu()
{
    //  Se despliegan las opciones.
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
    //  En esta funcion se procesa el ingreso de los nombres de jugadores. Además se asignan los turnos.
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

    //  Boton atras --> Deshace lo ingresado y vuelve al menu.
    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("--> ATRAS\n");
        drop_session(s);
        return MENU;
    }

    // Boton otro jugador --> Agrega al jugador a la lista y permite seguir ingresando mas.
    if (CheckCollisionPointRec(mouse, btnNewPlayer) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("--> OTRO_JUGADOR\n");
        if (s->input.keyCount > 2 && s->input.name[0] != '\0')
        {
            if (list_player(s))
            {
                reset_input(&s->input);
                s->qtyPlayers++;
            }
            else
            {
                printf("Error: Fallo al listar.\n");
                errorTimer = 2.0f;
                showError = 1;
            }
        }
        else
        {
            printf("Error: Nombre demasiado corto o vacio.\n");
            errorTimer = 2.0f;
            showError = 1;
        }
        return ENTER_PLAYERS;
    }

    // Boton comenzar --> Agrega al jugador actual si hay uno pendiente y procede a otorgar los turnos.
    if (CheckCollisionPointRec(mouse, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("--> COMENZAR\n");

        // Si hay un nombre en el input, lo intentamos agregar
        if (s->input.keyCount > 2  && s->input.name[0] != '\0')
        {
            if (list_player(s))
            {
                s->qtyPlayers++;
                reset_input(&s->input);
            }
            else
            {
                errorTimer = 2.0f;
                showError = 1;
                return ENTER_PLAYERS;
            }
        }

        // Validar que al menos haya un jugador
        if (s->qtyPlayers < 1)
        {
            errorTimer = 2.0f;
            showError = 1;
            printf("Error: Ingrese al menos 1 jugador.\n");
            return ENTER_PLAYERS;
        }
        queue_player(s);    // Con todos turnos asignados, encolamos.
        return ROUND;
    }

    // Si se intenta ingresar un nombre menor a 3 caracteres o si no se ingreso ningun nombre da error.
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
    //  Se muestran los turnos.

    Vector2 mouse = GetMousePosition();

    //  Boton atras --> Vuelve al ingreso de jugadores.
    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("--> ATRAS\n");
        return ENTER_PLAYERS;
    }

    //  Boton empezar --> Inicia la ronda de turnos.
    if(CheckCollisionPointRec(mouse, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("--> COMENZAR\n");
        return PLAYERS_READY;
    }
    return ROUND;
}

int update_ranking(tSession *s)
{
    Vector2 mouse = GetMousePosition();

    //  Boton atras --> Vuelve al menu.
    if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("--> ATRAS\n");
        return MENU;
    }

    return RANKING;
}

int update_player_ready(tSession *s, tPlays *p)
{
    //  Esta funcion procesa el inicio de la partida de un jugador o el abandono del mismo.

    tPlayer drop;   //  Variable basura.
    Vector2 mouse = GetMousePosition();

    if(colaVacia(&s->players_queue))    //  Si no hay mas jugadores por jugar termina.
        return END_SAVE;

    //  Boton rendirse --> Cancela la participacion del jugador.
    if (CheckCollisionPointRec(mouse, btnSurrender) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("--> RENDIRSE\n");
        s->qtyPlayers--;
        sacarDeCola(&s->players_queue, &drop, sizeof(tPlayer));
        finish_match(p);
        if (!colaVacia(&s->players_queue))
        {
            return PLAYERS_READY;
        }
        else
        {
            return END_SAVE;
        }
    }

    //  Boton empezar --> Comienza el juego para el jugador que le toca.
    if (CheckCollisionPointRec(mouse, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("--> COMENZAR\n");
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

    // Turno humano.
    if (p->curr_symbol == p->human_symbol)
    {
        for (row = 0; row < 3; row++)
        {
            for (col = 0; col < 3; col++)
            {
                Rectangle cell = grid[row][col];
                if (CheckCollisionPointRec(mouse, cell) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    printf("--> JG: [%d][%d]\n", row,col);
                    if (board[row][col] == 0)
                    {
                        res = human_playing(board, p,row,col);
                        switch(res)
                        {
                            case DRAW:
                            {
                                printf("Res.: Empate\n");
                                p->winner = DRAW;
                                return GAME_OVER;
                            }
                            case HUMAN_WIN:
                            {
                                printf("Res.: Gana jugador\n");
                                p->winner = HUMAN_WIN;
                                return GAME_OVER;
                            }
                            case PC_PLAY:
                            {
                                p->curr_symbol = p->pc_symbol;
                                return BOARD;
                            }
                        }
                    }
                }
            }
        }
    }
    //  Turno PC.
    else
    {
        // PC juega --> Intenta ganar, bloquear o jugada aleatoria.
        res = pc_playing(board,p);
        if (full_board(board))
            res = DRAW;
        switch(res)
        {
            case DRAW:
            {
                printf("Res.: Empate\n");
                p->winner = DRAW;
                return GAME_OVER;
            }
            case PC_WIN:
            {
                printf("Res.: Gana PC\n");
                p->winner = PC_WIN;
                return GAME_OVER;
            }
            case HUMAN_PLAY:
            {
                p->curr_symbol = p->human_symbol;
                return BOARD;
            }
        }
    }
    return BOARD;
}

int update_game_over(tSession *s, tPlays *p)
{
    Vector2 mouse = GetMousePosition();

    //  Boton siguiente --> Da paso al siguiente jugador en la cola.
    if (CheckCollisionPointRec(mouse, btnNext) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        printf("--> CONTINUAR\n");
        list_score(s, p);
        finish_match(p);
        return PLAYERS_READY;
    }
    // Mientras no haga clic, sigue en el estado de game over
    return GAME_OVER;
}

int update_end_save(tSession *s, tPlays *p)
{
    //  En esta funcion guardamos resultados, subimos a la API y armamos copia local.

    int index, success;
    int player_count = 0;
    tNodo *current = s->score_list;

    while (current != NULL)     //  Contamos cantidad de jugadores que quedaron.
    {
        player_count++;
        current = current->sig;
    }

    if (player_count == 0)
    {
        finish_match(p);
        drop_session(s);
        return MENU;
    }

    tPlayer *players_to_send = (tPlayer*)malloc(player_count * sizeof(tPlayer));
    if (players_to_send == NULL) {
        fprintf(stderr, "Error de memoria\n");
        vaciarLista(&s->score_list);
        return MENU;
    }

    current = s->score_list;
    index = 0;
    while (current != NULL && index < player_count)
    {
        tScore *score = (tScore*)current->info;
        players_to_send[index] = score->player;
        index++;
        current = current->sig;
    }
    success = post_players(players_to_send, player_count);
    if (success == 1)
        printf("Puntajes enviados a la API.\n");
    else
        printf("Error al enviar los scores a la API\n");

    save_game_report_list(&s->score_list);

    finish_match(p);
    drop_session(s);
    return MENU;
}

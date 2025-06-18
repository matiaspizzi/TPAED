#include <stdio.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "TDA_Lista.h"

#define MAX_DATE_LENGTH 32
#define MAX_FILENAME_LENGTH 100


void init_session(tSession *s)
{
    s->input.keyCount = 0;
    s->input.name[0] = '\0';
    crearLista(&s->players_list);
    crearCola(&s->players_queue);
    s->qtyPlayers = 0;
}

void drop_session(tSession *s)
{
    s->input.keyCount = 0;
    s->input.name[0] = '\0';
    vaciarLista(&s->players_list);
    vaciarCola(&s->players_queue);
    s->qtyPlayers = 0;
}

int reset_input(tInput *input)
{
    input->keyCount = 0;
    input->name[0] = '\0';
    return 1;
}

int list_player(tSession *s)
{
    //  Lista los nombres de los jugadores si cumplen ciertos requisitos.
    char name[MAX_BUFF_SIZE];
    strcpy(name, s->input.name);

    if(strlen(name) < 3 || strcmp(name,"  ") == 0)
        return ERROR;
    if (!ponerAlFinal(&s->players_list, &name, MAX_BUFF_SIZE))
    {
        printf("Error al enlistar.\n");
        return 0;
    }
    printf("%s enlistado.\n",name);
    return 1;
}

int queue_player(tSession *s)
{
    int i;
    char name[MAX_PLAYER_NAME];
    ordenarListaAleatorio(&s->players_list);
    printf("Lista ordenada aleatoriamente.\n");
    printf("Cargando cola turnos.\n");
    tPlayer p;
    p.points = 0;

    for(i = 0; i < s->qtyPlayers; i++)
    {
        verEnPosicion(&s->players_list,i,name,MAX_PLAYER_NAME);
        memcpy(p.name,name,MAX_PLAYER_NAME);
        ponerEnCola(&s->players_queue,&p,sizeof(tPlayer));
    }

    return OK;
}

int get_player(tPlayer *p, tSession *s)
{
    if(!verPrimeroCola(&s->players_queue,p,sizeof(tPlayer)))
        return ERROR;
    return OK;
}

void printString(const void *data)
{
    const char *text = (const char *)data;
    printf("%s\n", text);
}


void save_game_report(char name[MAX_PLAYER_NAME], int points, int board[3][3], int winner) {

    time_t now = time(NULL);
    struct tm *t = localtime(&now);


    char filename[MAX_FILENAME_LENGTH];
    snprintf(filename, sizeof(filename),
             "informe-juego_%04d-%02d-%02d-%02d-%02d-%02d.txt",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);


    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error al crear archivo de informe");
        return;
    }

    fprintf(file, "INFORME DE JUEGO\n");
    fprintf(file, "----------------\n");
    fprintf(file, "Jugador: %s\n", name);
    fprintf(file, "Puntos: %d\n", points);
    fprintf(file, "Resultado: %s\n\n", winner ? "VICTORIA" : "DERROTA");

    fprintf(file, "ESTADO DEL TABLERO:\n");
    fprintf(file, "-------------\n");
    for (int i = 0; i < 3; i++) {
        fprintf(file, "|");
        for (int j = 0; j < 3; j++) {
            char symbol = ' ';
            if (board[i][j] == 1) symbol = 'X';
            else if (board[i][j] == 2) symbol = 'O';

            fprintf(file, " %c |", symbol);
        }
        fprintf(file, "\n-------------\n");
    }

    fclose(file);
    printf("Informe guardado como: %s\n", filename);
}

int fifty_fifty()
{
    return (rand() % 2) + 1;  // Devuelve 1 o 2
}


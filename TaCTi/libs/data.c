#include <stdio.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "TDA_Lista.h"

#define MAX_DATE_LENGTH 32
#define MAX_FILENAME_LENGTH 100

int reset_input(tInput *input)
{
    input->name[0] = '\0';
    input->keyCount = 0;

    return 1;
}

int clear_input(tInput *input)
{
    input->name[0] = '\0';
    input->keyCount = 0;

    return 1;
}

int list_player(tLista *player_list, tInput *input)
{
    tPlayer player;
    player.points = 0;
    memcpy(&player.name, input->name,strlen(input->name));
    if (!ponerAlFinal(player_list, &player, sizeof(tPlayer)))
    {
        printf("Error al enlistar.\n");
        return 0;
    }
    printf("%s enlistado.\n",input->name);

    return 1;
}



int get_player(tPlayer *player, tLista *player_list)
{
    sacarAlFinal(player_list, player->name,strlen(player->name));
    return 1;
}

void printStringBuff(void* buff, const void *data)
{

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

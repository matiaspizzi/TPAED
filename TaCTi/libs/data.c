#include <stdio.h>
#include <string.h>

#include "data.h"

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
    vaciarLista(&s->score_list);
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
    {
        return ERROR;
    }
    if (!ponerAlFinal(&s->players_list, &name, MAX_BUFF_SIZE))
    {

        return ERROR;
    }
    printf("--------->%s\n",name);
    return OK;
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

int fifty_fifty()
{
    return (rand() % 2) + 1;  // Devuelve 1 o 2
}

int find_or_add_player(tPlayerSummary *summaries, int *count, const char *name, int points)
{
    for (int i = 0; i < *count; i++)
    {
        if (strcmp(summaries[i].name, name) == 0)
        {
            summaries[i].total_points += points;
            return i;
        }
    }

    // Si no existe, lo agrego
    strcpy(summaries[*count].name, name);
    summaries[*count].total_points = points;
    (*count)++;
    return *count - 1;
}


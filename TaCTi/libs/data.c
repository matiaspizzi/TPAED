#include <stdio.h>
#include <string.h>

#include "data.h"
#include "TDA_Lista.h"



void init_session(tSession *s)
{
    s->input.keyCount = 0;
    s->input.name[0] = '\0';
    crearLista(&s->players_list);
    crearCola(&s->players_queue);
    s->qtyPlayers = 0;
    s->roundMixed = 0;
    printf("Sesion iniciada.");
}



int reset_input(tInput *input)
{
    input->keyCount = 0;
    input->name[0] = '\0';
    return 1;
}


int list_player(tSession *s)
{
    if (!ponerAlFinal(&s->players_list, s->input.name, sizeof(tPlayer)))
    {
        printf("Error al enlistar.\n");
        return 0;
    }
    printf("%s enlistado.\n",s->input.name);
    return 1;
}

int set_round(tSession *s)
{
    if(!ordenarListaAleatorio(&s->players_list))
    {
        printf("Error al ordenar.\n");
        return ERROR;
    }
    printf("Ronda definida.\n");
    s->roundMixed = 1;
    return OK;
}

int drop_session(tSession *s)
{
    s->input.keyCount = 0;
    s->input.name[0] = '\0';
    if(!vaciarLista(&s->players_list) || !vaciarCola(&s->players_queue))
        return ERROR;
    s->qtyPlayers = 0;
    s->roundMixed = 0;
    printf("Sesion cancelada.");
    return OK;
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


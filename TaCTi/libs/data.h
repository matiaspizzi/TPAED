/**
    Esta librería contiene todo lo relacionado a la gestion de los datos.
**/

#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include "TDA_Lista.h"
#include "cola.h"

#define isVALIDCHAR(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z') || ((c) >= '0' && (c) <= '9') || (c) == ' ')

#define     MAX_BUFF_SIZE       100
#define     MAX_PLAYER_NAME     30

#define     OK      1
#define     ERROR   0

typedef struct
{
    char name[MAX_PLAYER_NAME];
    int points;
    char lastGame[32];
}tPlayer;

typedef struct
{
    char name[MAX_PLAYER_NAME];
    int points;
    tLista *members;
}tGroup;


typedef struct
{
    char name[MAX_PLAYER_NAME];
    int keyCount;
}tInput;


typedef struct
{
    tInput  input;
    tLista  players_list;
    tCola   players_queue;
    int     qtyPlayers;
    tLista  score_list;
}tSession;


typedef struct
{
    tLista  human;
    tLista  pc;
    int     curr_symbol;
    int     human_symbol;
    int     pc_symbol;
}tPlays;



void init_session           (tSession *s);
void drop_session           (tSession *s);
int  reset_input            (tInput *input);
int queue_player            (tSession *s);
int list_player             (tSession *s);
void printString            (const void *data);
int get_player              (tPlayer *p, tSession *s);
int fifty_fifty();



#endif // DATA_H_INCLUDED


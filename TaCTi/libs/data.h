/**
    Esta librería contiene todo lo relacionado a la gestion de los datos.
**/

#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include "lista.h"
#include "cola.h"


#define isVALIDCHAR(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z') || ((c) >= '0' && (c) <= '9') || (c) == ' ')

#define     MAX_BUFF_SIZE       100
#define     MAX_PLAYER_NAME     30
#define     MAX_DATE_LENGTH     32
#define     MAX_FILENAME_LENGTH 100

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
    tLista  score_list;
    tCola   players_queue;
    int     qtyPlayers;
}tSession;

typedef struct          //  Es para acumular los puntos de un mismo jugador.
{
    char name[MAX_PLAYER_NAME];
    int total_points;
} tPlayerSummary;



void    init_session            (tSession *s);
void    drop_session            (tSession *s);
int     reset_input             (tInput *input);
int     list_player             (tSession *s);
int     queue_player            (tSession *s);
int     get_player              (tPlayer *p, tSession *s);
void    printString             (const void *data);
int     fifty_fifty             ();
int     find_or_add_player      (tPlayerSummary *summaries, int *count, const char *name, int points);

#endif // DATA_H_INCLUDED


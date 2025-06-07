/**
    Esta librería contiene todo lo relacionado a la gestion de los datos.
**/

#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include "TDA_Lista.h"

#define     MAX_BUFF_SIZE       30
#define     MAX_PLAYER_NAME     30

typedef struct
{
    char name[MAX_PLAYER_NAME];
    int points;
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
    int cantPlayers;
}tInput;

extern tInput input;

void printString(const void *data);

#endif // DATA_H_INCLUDED

/**
    Esta librería contiene todo lo relacionado a la gestion de los datos.
**/


#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#define     MAX_BUFF_SIZE       30
#define     MAX_PLAYER_NAME     30

typedef struct
{
    char name[MAX_PLAYER_NAME];
    int points;
    char lastGame[32];
}tPlayer;





#endif // DATA_H_INCLUDED

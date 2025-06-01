#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include <stdlib.h>

typedef struct {
    char *ptr;
    size_t len;
} string;

typedef struct {
    char name[64];
    int points;
    char lastGame[32];
} Player;

#endif

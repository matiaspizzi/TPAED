#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

typedef struct {
    char *ptr;
    size_t len;
} string;

typedef struct {
    char name[64];
    int points;
    char lastGame[32];
} Player;

#endif // STRUCTS_H_INCLUDED

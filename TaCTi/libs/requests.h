#ifndef REQUESTS_H_INCLUDED
#define REQUESTS_H_INCLUDED

#include <curl/curl.h>

typedef struct {
    char *ptr;
    size_t len;
} cstring;

typedef struct {
    char name[64];
    int points;
    char lastGame[32];
} Player;

size_t get_players_callback(void *, size_t size, size_t, cstring *);

int get_players(CURL *, cstring *, const char *);

#endif


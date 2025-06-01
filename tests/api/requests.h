#ifndef REQUESTS_H_INCLUDED
#define REQUESTS_H_INCLUDED

#include <curl/curl.h>
#include "structs.h"

size_t get_players_callback(void *, size_t size, size_t, string *);

int get_players(CURL *, string *, const char *);

#endif


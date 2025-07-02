#ifndef REQUESTS_H_INCLUDED
#define REQUESTS_H_INCLUDED

#include "data.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 100
#define CURL_COMMAND "curl -s \"https://algoritmos-api.azurewebsites.net/api/TaCTi/funcion\""

char    *fetch_players_json     ();
int     parse_players           (const char *, tPlayer *, int);
int     get_players             (tPlayer **);
void    parse_json_players      (const char*, tPlayer *, int*);

char    *escape_quotes          (const char *input);
int     execute_curl            (const char *curl_command, char *response, size_t response_size);
char    *build_post_json        (const tPlayer *players, int player_count);
int     post_players            (const tPlayer *players, int player_count);
int     delete_all_players      ();
int     load_config();
#endif


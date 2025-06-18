#ifndef REQUESTS_H_INCLUDED
#define REQUESTS_H_INCLUDED

#include "data.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 100

char* escape_quotes(const char* input);
int execute_curl(const char* curl_command);
int get_players(tPlayer *players, int *player_count);
char* build_post_json(const tPlayer* players, int player_count);
int post_players(const tPlayer* players, int player_count);
int delete_all_players();

#endif


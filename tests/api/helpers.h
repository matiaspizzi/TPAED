#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "config.h"
#include "structs.h"

void parse_json_players(const char*, Player [], int*);

#endif

#include "requests.h"

char* fetch_players_json() {
    FILE *fp;
    char buffer[4096];
    char *result = NULL;
    size_t total_size = 0;

    fp = popen(CURL_COMMAND, "r");
    if (fp == NULL) {
        perror("Failed to execute curl");
        return NULL;
    }

    // Read response in chunks
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t chunk_size = strlen(buffer);
        char *temp = realloc(result, total_size + chunk_size + 1);
        if (temp == NULL) {
            free(result);
            pclose(fp);
            return NULL;
        }
        result = temp;
        memcpy(result + total_size, buffer, chunk_size);
        total_size += chunk_size;
        result[total_size] = '\0';
    }

    pclose(fp);
    return result;
}

// Function to parse JSON into Player array
int parse_players(const char *json_str, tPlayer *players, int max_players) {
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return 0;
    }

    int player_count = cJSON_GetArraySize(json);
    if (player_count > max_players) {
        player_count = max_players;
    }

    for (int i = 0; i < player_count; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "nombreJugador");
        cJSON *points = cJSON_GetObjectItemCaseSensitive(item, "puntaje");
        cJSON *last_game = cJSON_GetObjectItemCaseSensitive(item, "ultimaPartidaJugada");

        if (cJSON_IsString(name) && name->valuestring != NULL) {
            strncpy(players[i].name, name->valuestring, sizeof(players[i].name) - 1);
            players[i].name[sizeof(players[i].name) - 1] = '\0';
        }

        if (cJSON_IsNumber(points)) {
            players[i].points = points->valueint;
        }

        if (cJSON_IsString(last_game) && last_game->valuestring != NULL) {
            strncpy(players[i].lastGame, last_game->valuestring, sizeof(players[i].lastGame) - 1);
            players[i].lastGame[sizeof(players[i].lastGame) - 1] = '\0';
        }
    }

    cJSON_Delete(json);
    return player_count;
}

// Main function to get players
int get_players(tPlayer **players) {
    char *json_str = fetch_players_json();
    if (json_str == NULL) {
        return 0;
    }

    *players = malloc(MAX_PLAYERS * sizeof(tPlayer));
    if (*players == NULL) {
        free(json_str);
        return 0;
    }

    int player_count = parse_players(json_str, *players, MAX_PLAYERS);
    free(json_str);

    return player_count;
}

void parse_json_players(const char* json_string, tPlayer* players, int* cant) {
    cJSON *json_array = cJSON_Parse(json_string);
    if (json_array == NULL) {
        printf("Error al parsear JSON.\n");
        return;
    }

    int num_jugadores = cJSON_GetArraySize(json_array);
    *cant = num_jugadores;

    for (int i = 0; i < num_jugadores && i < MAX_PLAYERS; i++) {
        cJSON *jugador_json = cJSON_GetArrayItem(json_array, i);

        cJSON *name = cJSON_GetObjectItemCaseSensitive(jugador_json, "nombreJugador");
        cJSON *points = cJSON_GetObjectItemCaseSensitive(jugador_json, "puntaje");
        cJSON *date = cJSON_GetObjectItemCaseSensitive(jugador_json, "ultimaPartidaJugada");

        if (cJSON_IsString(name) && (name->valuestring != NULL))
            strncpy(players[i].name, name->valuestring, sizeof(players[i].name));

        if (cJSON_IsNumber(points))
            players[i].points = points->valueint;

        if (cJSON_IsString(date) && (date->valuestring != NULL))
            strncpy(players[i].lastGame, date->valuestring, sizeof(players[i].lastGame));
    }

    cJSON_Delete(json_array);
}

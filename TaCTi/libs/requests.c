#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "data.h"

#define API_URL "https://algoritmos-api.azurewebsites.net/api/TaCTi"
#define GROUP_CODE "funcion"
#define MAX_PLAYERS 100
#define MAX_DATE_LENGTH 32
#define CURL_TIMEOUT 10

char* escape_quotes(const char* input) {
    if (!input) return NULL;

    size_t len = strlen(input);
    char *output = malloc(len * 2 + 1);
    if (!output) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '"') {
            output[j++] = '\\';
        }
        output[j++] = input[i];
    }
    output[j] = '\0';
    return output;
}

int execute_curl(const char* curl_command, char* response, size_t response_size) {
    FILE *fp = popen(curl_command, "r");
    if (!fp) {
        perror("Error al ejecutar curl");
        return 0;
    }

    if (response && response_size > 0) {
        response[0] = '\0';
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            strncat(response, buffer, response_size - strlen(response) - 1);
        }
    } else {
        // Solo consumir la salida si no necesitamos respuesta
        while (fgetc(fp) != EOF);
    }

    return pclose(fp) == 0;
}

// ------------------------- GET -------------------------

int get_players(tPlayer *players, int *player_count) {
    char curl_command[256];
    snprintf(curl_command, sizeof(curl_command),
        "curl -s -X GET \"%s/%s\" --max-time %d",
        API_URL, GROUP_CODE, CURL_TIMEOUT);

    char response[4096] = {0};
    if (!execute_curl(curl_command, response, sizeof(response))) {
        return 0;
    }

    cJSON *json = cJSON_Parse(response);
    if (!json) {
        fprintf(stderr, "Error al parsear JSON: %s\n", cJSON_GetErrorPtr());
        return 0;
    }

    *player_count = cJSON_GetArraySize(json);
    if (*player_count > MAX_PLAYERS) {
        *player_count = MAX_PLAYERS;
    }

    for (int i = 0; i < *player_count; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);

        cJSON *name = cJSON_GetObjectItem(item, "nombreJugador");
        if (name && cJSON_IsString(name)) {
            strncpy(players[i].name, name->valuestring, MAX_PLAYER_NAME - 1);
            players[i].name[MAX_PLAYER_NAME - 1] = '\0';
        }

        cJSON *points = cJSON_GetObjectItem(item, "puntaje");
        if (points && cJSON_IsNumber(points)) {
            players[i].points = points->valueint;
        }

        cJSON *lastGame = cJSON_GetObjectItem(item, "ultimaPartidaJugada");
        if (lastGame && cJSON_IsString(lastGame)) {
            strncpy(players[i].lastGame, lastGame->valuestring, MAX_DATE_LENGTH - 1);
            players[i].lastGame[MAX_DATE_LENGTH - 1] = '\0';
        } else {
            players[i].lastGame[0] = '\0';
        }
    }

    cJSON_Delete(json);
    return 1;
}

// ------------------------- POST -------------------------

char* build_post_json(const tPlayer* players, int player_count) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "CodigoGrupo", GROUP_CODE);

    cJSON *players_array = cJSON_CreateArray();
    for (int i = 0; i < player_count; i++) {
        cJSON *player = cJSON_CreateObject();
        cJSON_AddStringToObject(player, "nombre", players[i].name);
        cJSON_AddNumberToObject(player, "puntos", players[i].points);
        cJSON_AddItemToArray(players_array, player);
    }
    cJSON_AddItemToObject(root, "Jugadores", players_array);

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str;
}

int post_players(const tPlayer* players, int player_count) {
    if (player_count <= 0 || player_count > MAX_PLAYERS) {
        fprintf(stderr, "Cantidad de jugadores invalida\n");
        return 0;
    }

    char *json_data = build_post_json(players, player_count);
    if (!json_data) {
        fprintf(stderr, "Error al construir JSON\n");
        return 0;
    }

    char *escaped_json = escape_quotes(json_data);
    free(json_data);
    if (!escaped_json) {
        fprintf(stderr, "Error al escapar JSON\n");
        return 0;
    }

    char curl_command[2048];
    snprintf(curl_command, sizeof(curl_command),
        "curl -s -X POST \"%s\" "
        "-H \"Content-Type: application/json\" "
        "-d \"%s\" "
        "--max-time %d",
        API_URL, escaped_json, CURL_TIMEOUT);

    free(escaped_json);

    return execute_curl(curl_command, NULL, 0);
}

// ------------------------- DELETE -------------------------

int delete_all_players() {
    char curl_command[256];
    snprintf(curl_command, sizeof(curl_command),
        "curl -s -X DELETE \"%s/%s\" --max-time %d",
        API_URL, GROUP_CODE, CURL_TIMEOUT);

    return execute_curl(curl_command, NULL, 0);
}

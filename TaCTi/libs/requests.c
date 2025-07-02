#include "requests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PLAYERS 100
#define CURL_TIMEOUT 10
#define CONFIG_FILE "config.txt"

static char API_URL[256];
static char GROUP_CODE[64];

// ------------------------- CONFIG -------------------------

int load_config()
{
    FILE *fp = fopen(CONFIG_FILE, "r");
    if (!fp) {
        perror("No se pudo abrir config.txt");
        return 0;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        char *eq = strchr(line, '=');
        if (!eq) continue;

        *eq = '\0';
        char *key = line;
        char *value = eq + 1;
        value[strcspn(value, "\r\n")] = '\0';

        if (strcmp(key, "API_URL") == 0) {
            strncpy(API_URL, value, sizeof(API_URL) - 1);
            API_URL[sizeof(API_URL) - 1] = '\0';
        } else if (strcmp(key, "GROUP_CODE") == 0) {
            strncpy(GROUP_CODE, value, sizeof(GROUP_CODE) - 1);
            GROUP_CODE[sizeof(GROUP_CODE) - 1] = '\0';
        }
    }

    fclose(fp);

    if (API_URL[0] == '\0' || GROUP_CODE[0] == '\0') {
        fprintf(stderr, "Faltan API_URL o GROUP_CODE en config.txt\n");
        return 0;
    }

    return 1;
}

// ------------------------- UTILS -------------------------

char *escape_quotes(const char *input) {
    if (!input) return NULL;

    size_t len = strlen(input);
    char *output = malloc(len * 2 + 1);
    if (!output) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '"') output[j++] = '\\';
        output[j++] = input[i];
    }

    output[j] = '\0';
    return output;
}

int execute_curl(const char *curl_command, char *response, size_t response_size) {
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
        while (fgetc(fp) != EOF);
    }

    return pclose(fp) == 0;
}

// ------------------------- GET -------------------------

char *fetch_players_json() {
    char curl_command[512];
    snprintf(curl_command, sizeof(curl_command),
             "curl -s -X GET \"%s/%s\"", API_URL, GROUP_CODE);

    FILE *fp = popen(curl_command, "r");
    if (!fp) {
        perror("Error al ejecutar curl");
        return NULL;
    }

    char buffer[4096];
    char *result = NULL;
    size_t total_size = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t chunk_size = strlen(buffer);
        char *temp = realloc(result, total_size + chunk_size + 1);
        if (!temp) {
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

int parse_players(const char *json_str, tPlayer *players, int max_players) {
    cJSON *json = cJSON_Parse(json_str);
    if (!json) return 0;

    int count = cJSON_GetArraySize(json);
    if (count > max_players) count = max_players;

    for (int i = 0; i < count; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "nombreJugador");
        cJSON *points = cJSON_GetObjectItemCaseSensitive(item, "puntaje");
        cJSON *last_game = cJSON_GetObjectItemCaseSensitive(item, "ultimaPartidaJugada");

        if (cJSON_IsString(name))
            strncpy(players[i].name, name->valuestring, sizeof(players[i].name) - 1);

        if (cJSON_IsNumber(points))
            players[i].points = points->valueint;

        if (cJSON_IsString(last_game))
            strncpy(players[i].lastGame, last_game->valuestring, sizeof(players[i].lastGame) - 1);
    }

    cJSON_Delete(json);
    return count;
}

int get_players(tPlayer **players) {
    char *json_str = fetch_players_json();
    if (!json_str) return 0;

    *players = malloc(MAX_PLAYERS * sizeof(tPlayer));
    if (!*players) {
        free(json_str);
        return 0;
    }

    int count = parse_players(json_str, *players, MAX_PLAYERS);
    free(json_str);
    return count;
}

// ------------------------- POST -------------------------

char *build_post_json(const tPlayer *players, int count) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "CodigoGrupo", GROUP_CODE);

    cJSON *arr = cJSON_CreateArray();
    for (int i = 0; i < count; i++) {
        cJSON *obj = cJSON_CreateObject();
        cJSON_AddStringToObject(obj, "nombre", players[i].name);
        cJSON_AddNumberToObject(obj, "puntos", players[i].points);
        cJSON_AddItemToArray(arr, obj);
    }

    cJSON_AddItemToObject(root, "Jugadores", arr);
    char *str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return str;
}

int post_players(const tPlayer *players, int count) {
    if (count <= 0 || count > MAX_PLAYERS) return 0;

    char *json_data = build_post_json(players, count);
    if (!json_data) return 0;

    char *escaped = escape_quotes(json_data);
    free(json_data);
    if (!escaped) return 0;

    char curl_command[2048];
    snprintf(curl_command, sizeof(curl_command),
             "curl -s -X POST \"%s\" -H \"Content-Type: application/json\" -d \"%s\" --max-time %d",
             API_URL, escaped, CURL_TIMEOUT);

    free(escaped);
    return execute_curl(curl_command, NULL, 0);
}

// ------------------------- DELETE -------------------------

int delete_all_players() {
    char curl_command[512];
    snprintf(curl_command, sizeof(curl_command),
             "curl -s -X DELETE \"%s/%s\" --max-time %d",
             API_URL, GROUP_CODE, CURL_TIMEOUT);

    return execute_curl(curl_command, NULL, 0);
}

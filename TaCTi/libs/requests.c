#include "requests.h"

#define API_URL "https://algoritmos-api.azurewebsites.net/api/TaCTi"
#define GROUP_CODE "funcion"
#define MAX_PLAYERS 100
#define MAX_DATE_LENGTH 32
#define CURL_TIMEOUT 10

char *escape_quotes(const char *input)
{
    if (!input)
        return NULL;

    size_t len = strlen(input);
    char *output = malloc(len * 2 + 1);
    if (!output)
        return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (input[i] == '"')
        {
            output[j++] = '\\';
        }
        output[j++] = input[i];
    }
    output[j] = '\0';
    return output;
}

int execute_curl(const char *curl_command, char *response, size_t response_size)
{
    FILE *fp = popen(curl_command, "r");
    if (!fp)
    {
        perror("Error al ejecutar curl");
        return 0;
    }

    if (response && response_size > 0)
    {
        response[0] = '\0';
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            strncat(response, buffer, response_size - strlen(response) - 1);
        }
    }
    else
    {
        // Solo consumir la salida si no necesitamos respuesta
        while (fgetc(fp) != EOF);
    }

    return pclose(fp) == 0;
}

// ------------------------- GET -------------------------

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

        if (cJSON_IsString(name) && (name->valuestring != NULL)) {
            strncpy(players[i].name, name->valuestring, sizeof(players[i].name) - 1);
            players[i].name[sizeof(players[i].name) - 1] = '\0';
        }
        if (cJSON_IsNumber(points))
            players[i].points = points->valueint;

        if (cJSON_IsString(date) && (date->valuestring != NULL)) {
            strncpy(players[i].lastGame, date->valuestring, sizeof(players[i].lastGame) - 1);
            players[i].lastGame[sizeof(players[i].lastGame) - 1] = '\0';
        }
    }

    cJSON_Delete(json_array);
}

// ------------------------- POST -------------------------

char *build_post_json(const tPlayer *players, int player_count)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "CodigoGrupo", GROUP_CODE);

    cJSON *players_array = cJSON_CreateArray();
    for (int i = 0; i < player_count; i++)
    {
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

int post_players(const tPlayer *players, int player_count)
{
    if (player_count <= 0 || player_count > MAX_PLAYERS)
    {
        fprintf(stderr, "Cantidad de jugadores invalida\n");
        return 0;
    }

    char *json_data = build_post_json(players, player_count);
    if (!json_data)
    {
        fprintf(stderr, "Error al construir JSON\n");
        return 0;
    }

    char *escaped_json = escape_quotes(json_data);
    free(json_data);
    if (!escaped_json)
    {
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

int delete_all_players()
{
    char curl_command[256];
    snprintf(curl_command, sizeof(curl_command),
             "curl -s -X DELETE \"%s/%s\" --max-time %d",
             API_URL, GROUP_CODE, CURL_TIMEOUT);

    return execute_curl(curl_command, NULL, 0);
}
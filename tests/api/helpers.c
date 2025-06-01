#include "helpers.h"

void parse_json_players(const char* json_string, Player players[], int* cant) {
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

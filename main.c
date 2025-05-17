#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON/cJSON.h"
#include "structs.h"
#include "requests.h"
#define MAX_PLAYERS 100
#define URL "https://algoritmos-api.azurewebsites.net/api/TaCTi/funcion"


void init_string(string *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "No se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

// Callback que CURL usa para escribir la respuesta en memoria
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

int main(void) {

    string response;
    init_string(&response);

    Player players[MAX_PLAYERS];
    int cant_players = 0;

    CURL *curl;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // URL de la API
        get_players(curl, &response, URL);

        parse_json_players(response.ptr, players, &cant_players);

        getchar();

        for (int i = 0; i < cant_players; i++) {
            printf("Jugador %d:\n", i+1);
            printf("  Nombre: %s\n", players[i].name);
            printf("  Puntaje: %d\n", players[i].points);
            printf("  Ultima partida: %s\n\n", players[i].lastGame);
        }

        getchar();

        // Limpieza
        curl_easy_cleanup(curl);
    }

    free(response.ptr);
    curl_global_cleanup();

    return 0;
}

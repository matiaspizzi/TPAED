#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON/cJSON.h"
#include <curl/curl.h>

#include "helpers.h"
#include "structs.h"
#include "requests.h"
#include "config.h"
void init_string(string *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "No se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

int main(void) {

    string response;
    init_string(&response);

    Player players[MAX_PLAYERS];
    int cant_players = 0, get_status = 0;

    CURL *curl;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {

        get_status = get_players(curl, &response, GET_PLAYERS_URL);

        if (get_status == 0) {

            parse_json_players(response.ptr, players, &cant_players);

            getchar();

            for (int i = 0; i < cant_players; i++) {
                printf("Jugador %d:\n", i+1);
                printf("  Nombre: %s\n", players[i].name);
                printf("  Puntaje: %d\n", players[i].points);
                printf("  Ultima partida: %s\n\n", players[i].lastGame);
            }

            getchar();
        }

        curl_easy_cleanup(curl);
    }

    free(response.ptr);
    curl_global_cleanup();

    return 0;
}

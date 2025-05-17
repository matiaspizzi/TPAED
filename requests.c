#include "requests.h"

size_t get_players_callback(void *ptr, size_t size, size_t nmemb, string *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "No se pudo reasignar memoria.\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->len = new_len;
    s->ptr[new_len] = '\0';
    return size * nmemb;
}

int get_players(CURL *curl, string *response,  const char *url){

    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, url);

    // CALLBACK
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_players_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    // HEADERS
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // SSL CERT
    curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");

    // Realiza la petición
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "Fallo en curl_easy_perform(): %s\n", curl_easy_strerror(res));
        getchar();
    } else {
        printf("Respuesta de la API:\n%s\n", response->ptr);
    }

    curl_slist_free_all(headers);

    return (res == CURLE_OK) ? 0 : 1;
}

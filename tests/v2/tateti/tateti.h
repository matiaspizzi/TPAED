#ifndef TATETI_H_INCLUDED
#define TATETI_H_INCLUDED

#define TAM 100

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    char nombre[TAM];
    int vecElem[5];
    int num_jugador;
    bool es_PC;
} tJugador;

int iniciar_partida(tJugador* jugadorA, tJugador* jugadorB, int* vecNumeros);

int inicializar_numeros(int* vecNumeros);

void inicializar_vector_en_cero(int* vec, int ce);

int partida(tJugador* jugadorA, tJugador* jugadorB, int* vecNumeros);

int juega_y_forma_tateti(tJugador* jugador, tJugador* jugadorInactivo, int* vecNumeros); /// aca está el vecRespuestas;

int decidir_ganador(tJugador* jugadorA, tJugador* jugadorB, int ganador);

int juega_pc(tJugador* jugador, int* vecNumeros, int* vecRespuestas);

int mostrar_vector(int* vec);

void intercambiar (void* a, void* b, size_t tamElem);

bool es_vector_nulo(int* vec);

int agregarNum(int* vec, int num);
int borrarNum(int* vec, int num);

int es_tateti(int* vecElem, int* vecRespuestas);
int descubrir_cantidad(int* vec);
int arma_fila(int magicNum, int* vecRespuestas);

int mostrar_tateti(int* vecJugador, int* vecJugadorInactivo );
#endif // TATETI_H_INCLUDED

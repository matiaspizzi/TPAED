#ifndef TATETI_MATRIZ_F_H_INCLUDED
#define TATETI_MATRIZ_F_H_INCLUDED

#include "../lista/lista.h"
#include<stdio.h>
#include<time.h>
#include<math.h>

#define GANA 1
#define GANA_PC 2
#define GANA_HUMANO 3
#define EMPATE 4

int iniciar_partida(int matriz[3][3], tLista* p_humano, tLista* p_pc);

int jugar_humano(int matriz[3][3], tLista* p_humano, tLista* p_pc);
int jugar_pc(int matriz[3][3],tLista* p_humano, tLista* p_pc);

int elegir_aleatorio(int  matriz[3][3], tLista* p_pc);

void mostrar_tateti(int matriz[3][3]);
int recorrer_tateti(int matriz[3][3], tLista* p);
int es_tateti(tLista* p);
int arma_fila(int magicNum, int* vecRespestas);

int matriz_completa(int matriz[3][3]);
#endif // TATETI_MATRIZ_F_H_INCLUDED

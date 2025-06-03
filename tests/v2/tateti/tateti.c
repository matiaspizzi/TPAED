#include "tateti.h"
#include <windows.h>
#include <unistd.h>

#define TODO_OK 0
#define JUGADOR_UNO 1
#define JUGADOR_DOS 2

#define COMBINA 1
#define NO_COMBINA 2

#define EMPATE 3

#define ERR_VEC 4

#define ROJO printf("\033[0;31m");
#define AZUL printf("\033[0;34m");
#define BLANCO printf("\033[0m");

int iniciar_partida(tJugador* jugadorA, tJugador* jugadorB, int* vecNumeros)
{
    system("cls");
    srand (time(NULL));
    inicializar_numeros(vecNumeros);

    jugadorA->es_PC = false;
    strcpy (jugadorA->nombre, "Humano");
    inicializar_vector_en_cero(jugadorA->vecElem, 5);

    jugadorB->es_PC = true;
    strcpy (jugadorB->nombre, "Computadora");
    inicializar_vector_en_cero(jugadorB->vecElem, 5);

    if((rand() % 2 ) == 1)
    {
        jugadorA->num_jugador = JUGADOR_UNO;
        jugadorB->num_jugador = JUGADOR_DOS;
    }
    else
    {
        jugadorB->num_jugador = JUGADOR_UNO;
        jugadorA->num_jugador = JUGADOR_DOS;
    }
    return TODO_OK;
}

void inicializar_vector_en_cero(int* vec, int ce)
{
    for (int i = 0; i < ce; i++)
    {
        *(vec + i) = 0;
    }
}
int inicializar_numeros(int* vecNumeros)
{
    int i, j, k;
    j = 1;
    k = 0;
    while(j<1000)
    {
        i = j;
        while(i < 5*j)
        {
            *(vecNumeros + k) = i;
            i *= 2;
            k++;
        }
        j*=10;
    }
    return 0;
}

int partida(tJugador* jugadorA, tJugador* jugadorB, int* vecNumeros)
{
    tJugador* jugadorActivo, *jugadorInactivo;
    if(jugadorA->num_jugador == JUGADOR_UNO)
    {
        jugadorActivo = jugadorA;
        jugadorInactivo = jugadorB;
        printf("Empieza Jugador: %s .\n",jugadorA->nombre);
    }
    else
    {
        jugadorActivo = jugadorB;
        jugadorInactivo = jugadorA;
        printf("Empieza Jugador: %s .\n",jugadorB->nombre);
    }
    while(!es_vector_nulo(vecNumeros))
    {
        if(juega_y_forma_tateti(jugadorActivo, jugadorInactivo, vecNumeros) == COMBINA)
        {
            return jugadorActivo->num_jugador;
        }
        intercambiar((void*)jugadorActivo, (void*)jugadorInactivo,sizeof(tJugador));
    }
    return EMPATE;
}

bool es_vector_nulo(int* vec)
{
    int i = 0;
    while(i < 9)
    {
        if(*(vec + i ) != 0)
        {
            return false;
        }
        i++;
    }
    return true;
}
int juega_y_forma_tateti(tJugador* jugador, tJugador* jugadorInactivo, int* vecNumeros) /// aca está el vecRespuestas;
{
    static int vecRespuestas[] = {7, 70, 111, 124, 222, 421, 444, 700};
    if (jugador->es_PC)
    {
        return juega_pc(jugador, vecNumeros, vecRespuestas);
    }
    printf("Elegir numero entre los disponibles: \n");
    //mostrar_vector(vecNumeros);
    mostrar_tateti(jugador->vecElem, jugadorInactivo->vecElem);
    int elegido;
    scanf("%d", &elegido);
    fflush(stdin);
    agregarNum(jugador->vecElem, elegido);
    borrarNum(vecNumeros, elegido);
    ///Despues realizar verificaciones, por ahora le creemos a todo.
    return es_tateti(jugador->vecElem, vecRespuestas);
}

int agregarNum(int* vec, int num)
{
    int i = 0;
    while (*(vec +i) != 0 && i < 5)
    {
        i++;
    }
    if(i == 5)
    {
        return ERR_VEC;
    }
    *(vec + i) = num;
    return TODO_OK;
}
int borrarNum(int* vec, int num)
{
    /// despues verificar;
    int i = 0;
    while (i < 8)
    {
        if ( * (vec + i) == num)
        {
            *(vec + i) = 0;
            return TODO_OK;
        }
        i++;
    }
    return ERR_VEC;
}

int mostrar_vector(int* vec)
{
    int i = 0;
    while(i < 9)
    {
        if(*(vec + i) != 0)
        {
            printf("[%d] ",*(vec + i));
        }
        i++;
    }
    return TODO_OK;
}
int decidir_ganador(tJugador* jugadorA, tJugador* jugadorB, int ganador);

int es_tateti(int* vecElem, int* vecRespuestas)
{
    int i, j, k, cantElem;
    i = 0;
    cantElem = descubrir_cantidad(vecElem);
    while (i < cantElem - 2)
    {
        j = i + 1;
        while (j < 4)
        {
            k = j + 1;
            while (k < 5)
            {
                if (arma_fila(vecElem[i] + vecElem[j] + vecElem[k], vecRespuestas) == COMBINA)
                {
                    return COMBINA;
                }
                k++;
            }
            j++;
        }
        i++;
    }
    return NO_COMBINA;
}

int descubrir_cantidad(int* vec)
{
    int i = 0;
    while (i < 5)
    {
        if(vec[i] == 0)
        {
            return i;
        }
        i++;
    }
    return i;
}

int arma_fila(int magicNum, int* vecRespuestas)
{
    for (int i = 0; i < 8; i++)
    {
        if(vecRespuestas[i] == magicNum)
        {
            return COMBINA;
        }
    }
    return NO_COMBINA;
}

int juega_pc(tJugador* jugador, int* vecNumeros, int* vecRespuestas)
{
    int num_rand;
    while(true)
    {
        num_rand = rand() % 8;
        if(*(vecNumeros + num_rand) != 0)
        {
            printf("Numero Elegido por PC: %d\n", *(vecNumeros + num_rand));
            agregarNum(jugador->vecElem, *(vecNumeros + num_rand));
            borrarNum(vecNumeros, *(vecNumeros + num_rand));
            return es_tateti(jugador->vecElem, vecRespuestas);
        }
    }
}

void intercambiar (void* a, void* b, size_t tamElem)
{
    void* aux = malloc(tamElem);
    memcpy(aux, a, tamElem);
    memcpy(a, b, tamElem);
    memcpy(b, aux, tamElem);
    free(aux);
}
int mostrar_tateti(int* vecJugador, int* vecJugadorInactivo )
{
    static int vecNumeros[] = {1,2,4,10,20,40,100,200,400};
    int j;
    for (int i = 0; i < 9; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if(*(vecNumeros + i) == * (vecJugador + j))
            {
                ROJO
                printf( "[%c] ",'X');
                BLANCO
                j+=5;
            }
            else
            {
                if(*(vecNumeros + i) == * (vecJugadorInactivo + j))
                {
                    AZUL
                    printf( "[%c] ",'O');
                    BLANCO
                    j+=5;
                }
            }
        }
        if (j == 5)
        {
            printf( "[%d] ",*(vecNumeros + i));
        }
        if((i + 1) % 3 == 0)
        {
            printf("\n");
        }
    }
    return TODO_OK;
}

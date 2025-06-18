#include "tateti_matriz_F.h"

int iniciar_partida(int matriz[3][3], tLista* p_humano, tLista* p_pc)
{
    srand(time(NULL));
    int valor;
    if(rand()%2 == 1)
    {
        puts("ARRANCA HUMANO");
        valor = jugar_humano(matriz, p_humano, p_pc);
    }
    else
    {
        puts("ARRANCA PC");
        {
            valor = jugar_pc(matriz, p_humano, p_pc);
        }
    }
    if( valor == EMPATE)
    {
        puts("EMPATE");
    }
    if( valor == GANA_HUMANO)
    {
        puts("GANA HUMANO");
    }
    if( valor == GANA_PC)
    {
        puts("GANA PC");
    }
    return 1;
}

int jugar_pc(int matriz[3][3],tLista* p_humano, tLista* p_pc)
{   int sacar_humano;
    if(recorrer_tateti(matriz, p_pc) != 0)
    {
        return GANA_PC;
    }
    else
    {
        if (recorrer_tateti(matriz, p_humano) == 0)
        {
            if( elegir_aleatorio(matriz, p_pc) == 0)
                return EMPATE;
        }
        else
        {
            sacarUltimoLista(p_humano,(void*)&sacar_humano, sizeof(int));
            ponerAlFinal(p_pc,(void*)&sacar_humano, sizeof(int));
        }
    }
    system("cls");
    mostrar_tateti(matriz);
    system("pause");
    system("cls");
    return jugar_humano(matriz, p_humano, p_pc);
}

int recorrer_tateti(int matriz[3][3], tLista* p)
{
    int valor;
    for(int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (matriz[i][j] == 0)
            {
                valor = (i + 1)* ( pow(10, j));
                ponerAlFinal(p, (void*)&valor, sizeof(int));
                if(es_tateti(p) == 1)
                {
                    matriz[i][j] = 2;
                    return GANA;
                }
                eliminarUltimoLista(p);
            }
        }
    }
    return 0;
}

int es_tateti(tLista* p)
{
    static int vec_resultados[] = { 6, 60, 111, 123, 222, 321, 333, 600 };

    tLista pri = *p;
    tLista i = *p;
    tLista j;
    tLista k;

    while ( i != NULL && i->sig != NULL )
    {
        j = i->sig;

        while ( j != NULL && j->sig != NULL)
        {
            k = j->sig;
            while ( k != NULL)
            {
                if(arma_fila(* ((int*) i->info) + * ((int*) j->info) + * ((int*) k->info), vec_resultados) == 1)
                {
                    *p = pri;
                    return 1;
                }
                k = k->sig;
            }
            j = j->sig;
        }
        i = i->sig;
    }
    *p = pri;
    return 0;
}
int arma_fila(int magicNum, int* vecRespestas)
{
    for(int i = 0; i < 8; i++)
    {
        if (vecRespestas[i] == magicNum)
        {
            return 1;
        }
    }
    return 0;
}


int jugar_humano(int matriz[3][3], tLista* p_humano, tLista* p_pc)
{
    if(matriz_completa(matriz))
    {
        return EMPATE;
    }
    mostrar_tateti(matriz);
    printf("Elegir casilla disponible: \n");
    int i;
    int j;
    printf("Elegir i: ");
    scanf("%d",&i);
    fflush(stdin);
    printf("Elegir j: ");
    scanf("%d",&j);
    fflush(stdin);
    matriz[i][j] = 1;
    int valor = (i + 1)* ( pow(10, j));
    ponerAlFinal(p_humano, (void*) &valor, sizeof(int));
    system("cls");
    mostrar_tateti(matriz);
    if(es_tateti(p_humano) == 1)
    {
        return GANA_HUMANO;
    }
    system("pause");
    return jugar_pc(matriz, p_humano, p_pc);
}

void mostrar_tateti(int matriz[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("[");
            if(matriz[i][j] == 0)
                printf("-");
            if(matriz[i][j] == 1)
                printf("X");
            if(matriz[i][j] == 2)
                printf("O");

            printf("] ");
        }
        printf("\n");
    }
}

int elegir_aleatorio(int  matriz[3][3], tLista* p_pc)
{
    int valor;
    int a = 0;
    int ai = 0;
    int aj = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if ( matriz[i][j] == 0)
            {
                if ( a == 0 )
                {
                    a = 1;
                    ai = i;
                    aj = j;
                }
                if ( rand() % 2 == 0)
                {
                    ai = i;
                    aj = j;
                }
            }
        }
    }
    if ( a != 0)
    {
        matriz[ai][aj] = 2;
        valor = (ai + 1)* ( pow(10, aj));
        ponerAlFinal(p_pc,(void*)&valor, sizeof(int));
        return 1;
    }
    return 0;
}

int matriz_completa(int matriz[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (matriz[i][j] == 0)
                return 0;
        }
    }
    return 1;
}

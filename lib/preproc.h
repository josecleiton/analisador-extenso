/*
** ARQUIVO QUE CARREGA TODAS AS BIBLIOTECAS UTILIZADAS
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/*
**  ERRO E EXCEPTION
**  macros para termíno abrupto do programa
*/
#define ERRO exit (3141592)
#define EXCEPTION 61023

/*
**  MALLOC
**  função de alocação
*/
void* MALLOC (size_t size)
{
    void* ptr;
    ptr = malloc (size);
    memset(ptr, 0, size);
    if (!ptr)
    {
        fprintf (stderr, "Memoria insuficiente.\n");
        ERRO;
    }
    return ptr;
}

/*
**  Redefinições de tipo
*/
typedef short int SU;
typedef long long int LLI;

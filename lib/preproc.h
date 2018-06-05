/*
** ARQUIVO QUE CARREGA TODAS AS BIBLIOTECAS UTILIZADAS
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
**  MALLOC
**  macro de alocação
*/
#define MALLOC(ptr, size) { \
    ptr = malloc (size); \
    if (! ptr) {\
        fprintf (stderr, "Memoria insuficiente.\n"); \
        ERRO; \
    } \
}

/*
**  ERRO E EXCEPTION
**  macros para termíno abrupto do programa
*/
#define ERRO exit (3141592)
#define EXCEPTION 61023

/*
**  Redefinições de tipo
*/
typedef short int SU;
typedef long long int LLI;
typedef unsigned char BOOL;

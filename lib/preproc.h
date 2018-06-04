/*    ARQUIVO QUE CARREGA TODAS AS BIBLIOTECAS USADAS */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MALLOC(ptr, size) { \
    ptr = malloc (size); \
    if (! ptr) {\
        fprintf (stderr, "Memoria insuficiente.\n"); \
        ERRO; \
    } \
}
#define ERRO exit (3141592)
typedef short int SU;
typedef long long int LLI;
typedef char BOOL;


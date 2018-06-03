/*    ARQUIVO QUE CARREGA TODAS AS BIBLIOTECAS USADAS */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define OPENFILE(ptr_file, file_name, type) { \
    ptr_file = fopen (file_name, type); \
    if (! ptr_file) { \
        fprintf (stderr, "Arquivo nao encontrado.\n"); \
        exit (2718); \
    } \
}
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


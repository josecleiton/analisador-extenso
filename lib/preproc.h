/*    ARQUIVO QUE CARREGA TODAS AS BIBLIOTECAS USADAS */
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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
typedef short int Int2B;


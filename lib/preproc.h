/*    ARQUIVO QUE CARREGA TODAS AS BIBLIOTECAS USADAS */
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define OPENFILE(ptr_file, file_name, type) { \
    ptr_file = fopen (file_name, type); \
    if (! ptr_file) { \
        fprintf (stderr, "arquivo nao encontrado\n"); \
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
typedef struct ordem Ordem;
typedef struct filanum FilaNum;
#ifndef STRUCTS
    #define STRUCTS
    struct ordem
    {
        char* nome;
        char* valor;
    };
    struct filanum
    {
        int classe;
        Ordem *info;
        FilaNum *prox;
    };
#endif
typedef short int Int2B;


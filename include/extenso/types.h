#ifndef EXTENSO_TYPES_H
#define EXTENSO_TYPES_H

#include "extenso/config.h"

/*
**  Term: guarda temporariamente uma linha do ARQ_DICT (<name>=<value>).
*/
typedef struct ordem
{
    char name[MAXWLEN];
    char value[MAXWLEN];
} Term;

/*
**  NumList: lista encadeada que carrega o número a ser analisado
**  semanticamente, casa decimal por casa decimal.
*/
typedef struct NumList
{
    short cls;
    Term *info;
    struct NumList *prev, *next;
} NumList;

/*
**  Index: vetor de posições (offsets) das linhas de um arquivo + seu tamanho.
*/
typedef struct index
{
    uint16_t *index;
    int tam;
} Index;

#endif

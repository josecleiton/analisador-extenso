#ifndef EXTENSO_TYPES_H
#define EXTENSO_TYPES_H

#include "extenso/config.h"

/*
**  Ordem: guarda temporariamente uma linha do ARQ_DICT (<nome>=<valor>).
*/
typedef struct ordem
{
    char nome[MAXWLEN];
    char valor[MAXWLEN];
} Ordem;

/*
**  ListaNum: lista encadeada que carrega o número a ser analisado
**  semanticamente, casa decimal por casa decimal.
*/
typedef struct ListaNum
{
    short classe;
    Ordem *info;
    struct ListaNum *ant, *prox;
} ListaNum;

/*
**  Index: vetor de posições (offsets) das linhas de um arquivo + seu tamanho.
*/
typedef struct index
{
    uint16_t *index;
    int tam;
} Index;

#endif

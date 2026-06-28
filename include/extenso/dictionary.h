#ifndef EXTENSO_DICTIONARY_H
#define EXTENSO_DICTIONARY_H

/*
**  Dicionário carregado em memória uma única vez (substitui o acesso por
**  fseek/fscanf ao ARQ_DICT a cada token).
**
**  Cada entrada guarda a linha crua do .cfg: `name` é tudo antes do '=' (pode
**  conter a forma composta "singular,plural", ex.: "milhao,milhoes") e `value`
**  é tudo depois do '='. O índice da entrada é exatamente o value do enum
**  `tokens` correspondente.
*/
#include "extenso/config.h"

typedef struct
{
    char name[MAXWLEN];
    char value[MAXWLEN];
} DictEntry;

typedef struct
{
    DictEntry *items;
    size_t n;            /* nº de entradas (era TAM_DICT) */
    size_t delim_start;  /* índice da 1ª entrada delimitadora (era INDEL) */
} Dictionary;

/* Carrega o dicionário; `n`/`delim_start` derivados do conteúdo. */
Dictionary *dictionary_load (const char *path);

/* Libera o dicionário. */
void dictionary_free (Dictionary *d);

#endif

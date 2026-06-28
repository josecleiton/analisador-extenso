#ifndef EXTENSO_TYPES_H
#define EXTENSO_TYPES_H

#include "extenso/config.h"

/*
**  Term: temporarily holds a line from ARQ_DICT (<name>=<value>).
*/
typedef struct ordem
{
    char name[MAXWLEN];
    char value[MAXWLEN];
} Term;

/*
**  NumList: linked list that carries the number to be analyzed semantically,
**  decimal place by decimal place.
*/
typedef struct NumList
{
    short cls;
    Term *info;
    struct NumList *prev, *next;
} NumList;

/*
**  Index: array of line positions (offsets) of a file + its size.
*/
typedef struct index
{
    uint16_t *index;
    int tam;
} Index;

#endif

#ifndef EXTENSO_NUM_LIST_H
#define EXTENSO_NUM_LIST_H

/*
**  ADT for the linked list (NumList) that holds the number being analyzed.
*/
#include "extenso/context.h"
#include "extenso/types.h"

/* Queue-style insertion into the context list. */
void listAppend (Context *ctx, uint16_t i, char *name, char *value);

/* Node of the next number (cls < MIL) in the queue. */
NumList *lastNumberNode (NumList *inicio);

/* Frees the context list. */
void listFree (Context *ctx);

/* Counts the nodes of the context list. */
int listCount (Context *ctx);

#endif

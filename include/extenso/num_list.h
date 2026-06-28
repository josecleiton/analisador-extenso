#ifndef EXTENSO_NUM_LIST_H
#define EXTENSO_NUM_LIST_H

/*
**  ADT da lista encadeada (NumList) que guarda o número em análise.
*/
#include "extenso/types.h"
#include "extenso/context.h"

/* Inserção como fila na lista do contexto. */
void listAppend (Context *ctx, uint16_t i, char *name, char *value);

/* Nó do próximo número (cls < MIL) na fila. */
NumList *lastNumberNode (NumList *inicio);

/* Libera a lista do contexto. */
void listFree (Context *ctx);

/* Conta os nós da lista do contexto. */
int listCount (Context *ctx);

#endif

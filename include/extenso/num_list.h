#ifndef EXTENSO_NUM_LIST_H
#define EXTENSO_NUM_LIST_H

/*
**  ADT da lista encadeada (ListaNum) que guarda o número em análise.
*/
#include "extenso/types.h"
#include "extenso/context.h"

/* Inserção como fila na lista do contexto. */
void listaInsere (Context *ctx, uint16_t i, char *nome, char *valor);

/* Nó do próximo número (classe < MIL) na fila. */
ListaNum *pegaProxNum (ListaNum *inicio);

/* Libera a lista do contexto. */
void listaLibera (Context *ctx);

/* Conta os nós da lista do contexto. */
int listaCount (Context *ctx);

#endif

#ifndef EXTENSO_NUM_LIST_H
#define EXTENSO_NUM_LIST_H

/*
**  ADT da lista encadeada (ListaNum) que guarda o número em análise.
*/
#include "extenso/types.h"

/* Inserção como fila. */
void listaInsere (uint16_t i, char *nome, char *valor);

/* Nó do próximo número (classe < MIL) na fila. */
ListaNum *pegaProxNum (ListaNum *inicio);

/* Libera a lista global. */
void listaLibera (void);

/* Conta os nós da lista global. */
int listaCount (void);

#endif

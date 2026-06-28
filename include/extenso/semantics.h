#ifndef EXTENSO_SEMANTICS_H
#define EXTENSO_SEMANTICS_H

/*
**  Análise semântica: valida ordem/plural/conjunção de um número por extenso.
*/
#include "extenso/types.h"
#include "extenso/context.h"

/* Analisa o significado da expressão. */
bool analiSemantica (Context *ctx);

/* Analisa a centena/dezena/unidade. */
bool semUnidade (Context *ctx, ListaNum **inicio);

/* Analisa o plural de ordens (>= MIL). */
void pluralOrdem (Context *ctx, ListaNum *inicio);

/* Ordem do número apontado por inicio. */
uint16_t pegaOrdem (ListaNum *inicio);

/* Próxima "classe" apontada por inicio. */
uint16_t pegaProxClasse (ListaNum *inicio);

#endif

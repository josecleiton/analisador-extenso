#ifndef EXTENSO_SEMANTICS_H
#define EXTENSO_SEMANTICS_H

/*
**  Análise semântica: valida ordem/plural/conjunção de um número por extenso.
*/
#include "extenso/context.h"
#include "extenso/types.h"

/* Analisa o significado da expressão. */
bool checkSemantics (Context *ctx);

/* Analisa a centena/dezena/unidade. */
bool checkUnit (Context *ctx, NumList **inicio);

/* Analisa o plural de ordens (>= MIL). */
void checkPlural (Context *ctx, NumList *inicio);

/* Term do número apontado por inicio. */
uint16_t orderOf (NumList *inicio);

/* Próxima "cls" apontada por inicio. */
uint16_t nextClass (NumList *inicio);

#endif

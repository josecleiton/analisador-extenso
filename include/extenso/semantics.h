#ifndef EXTENSO_SEMANTICS_H
#define EXTENSO_SEMANTICS_H

/*
**  Semantic analysis: validates order/plural/conjunction of a spelled-out number.
*/
#include "extenso/context.h"
#include "extenso/types.h"

/* Analyzes the meaning of the expression. */
bool checkSemantics (Context *ctx);

/* Analyzes the hundreds/tens/units. */
bool checkUnit (Context *ctx, NumList **inicio);

/* Analyzes the plural of magnitudes (>= MIL). */
void checkPlural (Context *ctx, NumList *inicio);

/* Magnitude of the number pointed to by inicio. */
uint16_t orderOf (NumList *inicio);

/* Next "cls" pointed to by inicio. */
uint16_t nextClass (NumList *inicio);

#endif

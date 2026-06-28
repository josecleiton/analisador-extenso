#ifndef EXTENSO_PARSER_H
#define EXTENSO_PARSER_H

/*
**  Avaliador por descida recursiva. Dirige o lexer, chama as operações de
**  bignum e devolve o resultado como string de dígitos.
*/
#include "extenso/context.h"

/* Gatilho de partida: avalia ctx->EXP e devolve o resultado por extenso. */
char *expParsingStart (Context *ctx);

/* Soma/subtrai termos. */
void expResTermo (Context *ctx, char *resposta);

/* Multiplica/divide/mod/eleva fatores. */
void expResFator (Context *ctx, char *resposta);

/* Resolve o fatorial de um fator. */
void expResFatorial (Context *ctx, char *resposta);

/* Resolve uma expressão entre parênteses. */
void expResParenteses (Context *ctx, char *resposta);

/* Devolve o valor numérico das expressões por extenso. */
void atomo (Context *ctx, char *resposta);

#endif

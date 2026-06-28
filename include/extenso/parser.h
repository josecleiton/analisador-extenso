#ifndef EXTENSO_PARSER_H
#define EXTENSO_PARSER_H

/*
**  Avaliador por descida recursiva. Dirige o lexer, chama as operações de
**  bignum e devolve o result como string de dígitos.
*/
#include "extenso/context.h"

/* Gatilho de partida: avalia ctx->cursor e devolve o result por extenso. */
char *evalExpr (Context *ctx);

/* Soma/subtrai termos. */
void parseTerm (Context *ctx, char *answer);

/* Multiplica/divide/mod/eleva fatores. */
void parseFactor (Context *ctx, char *answer);

/* Resolve o bigFactorial de um fator. */
void parseFactorial (Context *ctx, char *answer);

/* Resolve uma expressão entre parênteses. */
void parseParen (Context *ctx, char *answer);

/* Devolve o value numérico das expressões por extenso. */
void parseAtom (Context *ctx, char *answer);

#endif

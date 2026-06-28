#ifndef EXTENSO_PARSER_H
#define EXTENSO_PARSER_H

/*
**  Recursive-descent evaluator. Drives the lexer, calls the bignum operations
**  and returns the result as a digit string.
*/
#include "extenso/context.h"

/* Entry trigger: evaluates ctx->cursor and returns the result in spelled-out form. */
char *evalExpr (Context *ctx);

/* Adds/subtracts terms. */
void parseTerm (Context *ctx, char *answer);

/* Multiplies/divides/mods/raises factors. */
void parseFactor (Context *ctx, char *answer);

/* Resolves the bigFactorial of a factor. */
void parseFactorial (Context *ctx, char *answer);

/* Resolves an expression within parentheses. */
void parseParen (Context *ctx, char *answer);

/* Returns the numeric value of the spelled-out expressions. */
void parseAtom (Context *ctx, char *answer);

#endif

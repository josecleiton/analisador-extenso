#ifndef EXTENSO_LEXER_H
#define EXTENSO_LEXER_H

/*
**  Lexical analyzer: breaks the expression into tokens by consulting the
**  dictionary and enqueues the numbers found in the context list.
*/
#include "extenso/config.h"
#include "extenso/context.h"

/* Matches the current word at ctx->cursor against the dictionary. */
void nextToken (Context *ctx);

/* Skips the word being analyzed. */
void skipWord (Context *ctx);

/* True if the next token is a delimiter. */
bool nextIsDelimiter (Context *ctx);

/* For compound magnitudes, evaluates plural/singular and enqueues the inserted form. */
bool matchPlural (Context *ctx, int i, char *s);

/* Inserts a hyphen in compound delimiters (e.g.: "dividido por" -> "dividido-por"). */
void joinCompoundDelim (Context *ctx, int *k, char *temp);

/* Converts ctx->cursor to lowercase. */
void lowercaseExpr (Context *ctx);

#endif

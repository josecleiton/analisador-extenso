#ifndef EXTENSO_LEXER_H
#define EXTENSO_LEXER_H

/*
**  Analisador léxico: quebra a expressão em tokens consultando o dicionário
**  e enfileira os números encontrados na lista do contexto.
*/
#include "extenso/config.h"
#include "extenso/context.h"

/* Casa a palavra corrente de ctx->cursor com o dicionário. */
void nextToken (Context *ctx);

/* Pula a palavra em análise. */
void skipWord (Context *ctx);

/* Verdadeiro se o próximo token for um delimitador. */
bool nextIsDelimiter (Context *ctx);

/* Em ordens compostas, avalia plural/singular e enfila a forma inserida. */
bool matchPlural (Context *ctx, int i, char *s);

/* Coloca hífen em delimitadores compostos (ex.: "dividido por" -> "dividido-por"). */
void joinCompoundDelim (Context *ctx, int *k, char *temp);

/* Converte ctx->cursor para minúsculas. */
void lowercaseExpr (Context *ctx);

#endif

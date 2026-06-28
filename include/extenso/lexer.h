#ifndef EXTENSO_LEXER_H
#define EXTENSO_LEXER_H

/*
**  Analisador léxico: quebra a expressão em tokens consultando o dicionário
**  e enfileira os números encontrados na lista do contexto.
*/
#include "extenso/config.h"
#include "extenso/context.h"

/* Casa a palavra corrente de ctx->EXP com o dicionário. */
void pegaToken (Context *ctx);

/* Pula a palavra em análise. */
void ajustaEXP (Context *ctx);

/* Verdadeiro se o próximo token for um delimitador. */
bool verificaProxToken (Context *ctx);

/* Em ordens compostas, avalia plural/singular e enfila a forma inserida. */
bool resPlural (Context *ctx, int i, char *s);

/* Coloca hífen em delimitadores compostos (ex.: "dividido por" -> "dividido-por"). */
void ajustaDelim (Context *ctx, int *k, char *temp);

/* Converte ctx->EXP para minúsculas. */
void strToLower (Context *ctx);

#endif

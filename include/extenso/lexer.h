#ifndef EXTENSO_LEXER_H
#define EXTENSO_LEXER_H

/*
**  Analisador léxico: quebra a expressão em tokens consultando o dicionário
**  e enfileira os números encontrados na lista.
*/
#include "extenso/config.h"

/* Casa a palavra corrente de EXP com o dicionário, atualizando token/tipo. */
void pegaToken (void);

/* Pula a palavra em análise. */
void ajustaEXP (void);

/* Verdadeiro se o próximo token for um delimitador. */
bool verificaProxToken (void);

/* Em ordens compostas, avalia plural/singular e enfila a forma inserida. */
bool resPlural (int i, char *s);

/* Coloca hífen em delimitadores compostos (ex.: "dividido por" -> "dividido-por"). */
void ajustaDelim (int *k, char *temp);

/* Converte EXP para minúsculas. */
void strToLower (void);

#endif

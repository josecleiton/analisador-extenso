#ifndef EXTENSO_CLI_H
#define EXTENSO_CLI_H

/*
**  Interface de linha de comando: menu, modos teclado/arquivo, exibição.
*/
#include "extenso/config.h"
#include "extenso/context.h"

/* Menu que dá início à análise. */
int runMenu (Context *ctx);

/* Imprime o manual de uso. */
void printHelp (void);

/* Dispara a análise lendo de inPath e escrevendo em outPath. */
int runFile (Context *ctx, const char *inPath, const char *outPath);

/* Mostra o arquivo de result. */
void printResults (void);

/* Tamanho da maior linha do stream. */
size_t longestLine (FILE *stream);

/* Método portável de limpar a tela. */
void clearScreen (void);

#endif

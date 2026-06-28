#ifndef EXTENSO_UTIL_H
#define EXTENSO_UTIL_H

/*
**  Utilidades: alocação com tratamento de erro, abertura de arquivo e
**  indexação de linhas de arquivo.
*/
#include "extenso/config.h"
#include "extenso/types.h"

/* calloc abortando em falha. */
void *alloc (const size_t count, const size_t blockSize);

/* fopen abortando em falha. */
FILE *openFile (const char filename[], const char type[]);

/* Reporta o errno e aborta o programa (falha irrecuperável). */
int abortWithLog (const bool msg);

/* Devolve as posições (offsets) do início de cada linha de `in`. */
Index buildLineIndex (FILE *in, int limite);

#endif

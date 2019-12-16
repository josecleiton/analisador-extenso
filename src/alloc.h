#ifndef _ALLOC_H
#define _ALLOC_H

/*
** BIBLIOTECA SIMPLES PARA ALOCAÇÃO DE MEMÓRIA E TRATAMENTO DE ERROS
*/

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _1KB 1024 /* 2^10 BYTES */

void* alloc(const size_t count, const size_t blockSize);
FILE* openFile(const char filename[], const char type[]);
int abortWithLog(const bool msg);

#endif

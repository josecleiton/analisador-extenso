#ifndef _ALLOC_H
#define _ALLOC_H

/*
** BIBLIOTECA SIMPLES PARA ALOCAÇÃO DE MEMÓRIA E TRATAMENTO DE ERROS
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

void* alloc (size_t count, size_t blockSize);
FILE* openFile (const char file_name[], const char type[]);
int abortWithLog (bool msg);

#endif
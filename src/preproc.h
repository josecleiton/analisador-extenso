#ifndef _PREPROC_H_
#define _PREPROC_H_
/*
** ARQUIVO QUE CARREGA TODAS AS BIBLIOTECAS UTILIZADAS
*/
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
**  ERRO E EXCEPTION
**  macros para termíno abrupto do programa
*/
#define ERRO exit(3141592)
#define EXCEPTION 61023

/*
**  Redefinições de tipo
*/
typedef short int SU;
typedef long long int LLI;
#endif
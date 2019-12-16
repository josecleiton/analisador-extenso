#ifndef _OPERACOES_H_
#define _OPERACOES_H_

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"

#define MAX 10000

/*
**  OPERAÇÕES COM NÚMEROS GUARDADOS EM STRING (BIG INTEGERS)
**  POR EXEMPLO, O NUMERO 154897
**  1 5 4 8 9 7 \0 <- VETOR EM MEMÓRIA
**  0 1 2 3 4 5 6  <- INDICES DO VETOR DE CARACTRES
*/

char* somar(char op1[], char op2[]); /* SOMA DE BIGINTEGERS */
char* subtrair(char a[], char b[]);  /* SUBTRAÇÃO DE BIGINTEGERS */
char* completaMenor(char a[], char b[],
                    char* menor);      /* COLOCA ZEROS NA MENOR STRING */
char* multiplicar(char a[], char b[]); /* MULTIPLICAÇÃO DE BIGINTEGERS */
char* unsigneDiv(char a[], char D[], bool MOD); /* DIVISÃO E MODULO SEM SINAL */
char* unExpo(char a[], char b[]); /* EXPONENCIAL SEM SINAL EM BIGINTEGERS */
char* fatorial(char a[]);         /* FATORIAL DE BIGINTEGERS */
bool strIsDigit(
    char a[]); /* VERIFICA SE A STRING É TODA NUMERICA (ASCII OU INT) */
int fatorialMultiplicador(int a, char fat[], int limit);
bool char2int(char a[]);            /* RETIRA '0' EM TODOS OS CARACTERES */
bool int2char(char a[], int tam);   /* ADICIONA '0' EM TODOS OS CARACTERES */
bool inverte(char a[]);             /* strrev (apenas para Janelas OS) */
int maior(int a, int b);            /* RETORNA O MAIOR ENTRE OS DOIS INTEIROS */
int menor(int a, int b);            /* RETORNA O MENOR ENTRE OS DOIS INTEIROS */
bool strCmpNum(char x[], char y[]); /* COMPARA OS NUMEROS NAS STRINGS */
int countDigits(long long x);
void trataZeros(char** ptrNumber); /* IGNORA OS ZEROS E RETORNA EM PTRNUMBER UMA
                                      NOVA STRING SEM ELES */
void ignoraZero(int narg,
                ...); /* ESPERA QUANTOS ARGUMENTOS E UM PONTEIRO PARA STRING */

/*
**  GERENCIA MEMORIA ENTRE O RESULTADO E OS OPERADORES DAS FUNÇÕES
*/
bool memswap(char a[], char b[], char* (*f)(char*, char*));
bool memswapDiv(char a[], char b[], bool mod, char* (*f)(char*, char*, bool));
void charswap(char* a, char* b);
#endif

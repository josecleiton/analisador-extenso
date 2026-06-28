#ifndef EXTENSO_BIGNUM_H
#define EXTENSO_BIGNUM_H

/*
**  OPERAÇÕES COM NÚMEROS GUARDADOS EM STRING (BIG INTEGERS)
**  POR EXEMPLO, O NUMERO 154897
**  1 5 4 8 9 7 \0 <- VETOR EM MEMÓRIA
**  0 1 2 3 4 5 6  <- INDICES DO VETOR DE CARACTERES
*/
#include <stdarg.h>
#include "extenso/config.h"
#include "extenso/util.h"

char *somar (char op1[], char op2[]);          /* SOMA DE BIGINTEGERS */
char *subtrair (char a[], char b[]);            /* SUBTRAÇÃO DE BIGINTEGERS */
char *completaMenor (char a[], char b[], char *menor); /* ZEROS NA MENOR STRING */
char *multiplicar (char a[], char b[]);         /* MULTIPLICAÇÃO DE BIGINTEGERS */
char *unsigneDiv (char a[], char D[], bool MOD); /* DIVISÃO E MODULO SEM SINAL */
char *unExpo (char a[], char b[]);              /* EXPONENCIAL SEM SINAL */
char *fatorial (char a[]);                      /* FATORIAL DE BIGINTEGERS */
bool strIsDigit (char a[]);                     /* STRING É TODA NUMERICA? */
int fatorialMultiplicador (int a, char fat[], int limit);
bool char2int (char a[]);                       /* RETIRA '0' DE CADA CARACTERE */
bool int2char (char a[], int tam);              /* ADICIONA '0' EM CADA CARACTERE */
bool inverte (char a[]);                        /* strrev */
int maior (int a, int b);                       /* MAIOR ENTRE DOIS INTEIROS */
int menor (int a, int b);                       /* MENOR ENTRE DOIS INTEIROS */
bool strCmpNum (char x[], char y[]);            /* COMPARA OS NUMEROS NAS STRINGS */
int countDigits (long long x);
void trataZeros (char **ptrNumber);             /* REMOVE ZEROS À ESQUERDA */
void ignoraZero (int narg, ...);                /* AVANÇA PONTEIROS PASSADO OS ZEROS */

/* GERENCIA MEMORIA ENTRE O RESULTADO E OS OPERADORES DAS FUNÇÕES */
bool memswap (char a[], char b[], char *(*f)(char *, char *));
bool memswapDiv (char a[], char b[], bool mod, char *(*f)(char *, char *, bool));
void charswap (char *a, char *b);

#endif

#ifndef EXTENSO_BIGNUM_H
#define EXTENSO_BIGNUM_H

/*
**  OPERAÇÕES COM NÚMEROS GUARDADOS EM STRING (BIG INTEGERS)
**  POR EXEMPLO, O NUMERO 154897
**  1 5 4 8 9 7 \0 <- VETOR EM MEMÓRIA
**  0 1 2 3 4 5 6  <- INDICES DO VETOR DE CARACTERES
*/
#include "extenso/config.h"
#include "extenso/util.h"
#include <stdarg.h>

char *bigAdd (char op1[], char op2[]);              /* SOMA DE BIGINTEGERS */
char *bigSub (char a[], char b[]);                  /* SUBTRAÇÃO DE BIGINTEGERS */
char *padToWidth (char a[], char b[], char *menor); /* ZEROS NA MENOR STRING */
char *bigMul (char a[], char b[]);                  /* MULTIPLICAÇÃO DE BIGINTEGERS */
char *bigDivMod (char a[], char D[], bool MOD);     /* DIVISÃO E MODULO SEM SINAL */
char *bigPow (char a[], char b[]);                  /* EXPONENCIAL SEM SINAL */
char *bigFactorial (char a[]);                      /* FATORIAL DE BIGINTEGERS */
bool isAllDigits (char a[]);                        /* STRING É TODA NUMERICA? */
int factMulSmall (int a, char fat[], int limit);
bool digitsToValues (char a[]);            /* RETIRA '0' DE CADA CARACTERE */
bool valuesToDigits (char a[], int tam);   /* ADICIONA '0' EM CADA CARACTERE */
bool reverseStr (char a[]);                /* strrev */
int maior (int a, int b);                  /* MAIOR ENTRE DOIS INTEIROS */
int menor (int a, int b);                  /* MENOR ENTRE DOIS INTEIROS */
bool numGreaterEqual (char x[], char y[]); /* COMPARA OS NUMEROS NAS STRINGS */
void stripLeadingZeros (char **ptrNumber); /* REMOVE ZEROS À ESQUERDA */
void advancePastZeros (int narg, ...);     /* AVANÇA PONTEIROS PASSADO OS ZEROS */

/* GERENCIA MEMORIA ENTRE O RESULTADO E OS OPERADORES DAS FUNÇÕES */
bool applyInto (char a[], char b[], char *(*f) (char *, char *));
bool applyDivInto (char a[], char b[], bool mod, char *(*f) (char *, char *, bool));
void swapChars (char *a, char *b);

#endif

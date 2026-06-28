#ifndef EXTENSO_BIGNUM_H
#define EXTENSO_BIGNUM_H

/*
**  OPERATIONS ON NUMBERS STORED IN STRINGS (BIG INTEGERS)
**  FOR EXAMPLE, THE NUMBER 154897
**  1 5 4 8 9 7 \0 <- ARRAY IN MEMORY
**  0 1 2 3 4 5 6  <- INDICES OF THE CHARACTER ARRAY
*/
#include "extenso/config.h"
#include "extenso/util.h"
#include <stdarg.h>

char *bigAdd (char op1[], char op2[]);              /* SUM OF BIGINTEGERS */
char *bigSub (char a[], char b[]);                  /* SUBTRACTION OF BIGINTEGERS */
char *padToWidth (char a[], char b[], char *menor); /* ZEROS ON THE SHORTER STRING */
char *bigMul (char a[], char b[]);                  /* MULTIPLICATION OF BIGINTEGERS */
char *bigDivMod (char a[], char D[], bool MOD);     /* UNSIGNED DIVISION AND MODULO */
char *bigPow (char a[], char b[]);                  /* UNSIGNED EXPONENTIATION */
char *bigFactorial (char a[]);                      /* FACTORIAL OF BIGINTEGERS */
bool isAllDigits (char a[]);                        /* IS THE STRING ALL NUMERIC? */
int factMulSmall (int a, char fat[], int limit);
bool digitsToValues (char a[]);            /* SUBTRACTS '0' FROM EACH CHARACTER */
bool valuesToDigits (char a[], int tam);   /* ADDS '0' TO EACH CHARACTER */
bool reverseStr (char a[]);                /* strrev */
int maior (int a, int b);                  /* LARGER OF TWO INTEGERS */
int menor (int a, int b);                  /* SMALLER OF TWO INTEGERS */
bool numGreaterEqual (char x[], char y[]); /* COMPARES THE NUMBERS IN THE STRINGS */
void stripLeadingZeros (char **ptrNumber); /* REMOVES LEADING ZEROS */
void advancePastZeros (int narg, ...);     /* ADVANCES POINTERS PAST THE ZEROS */

/* MANAGES MEMORY BETWEEN THE RESULT AND THE OPERANDS OF THE FUNCTIONS */
bool applyInto (char a[], char b[], char *(*f) (char *, char *));
bool applyDivInto (char a[], char b[], bool mod, char *(*f) (char *, char *, bool));
void swapChars (char *a, char *b);

#endif

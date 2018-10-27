#ifndef _OPERACOES_H_
#define _OPERACOES_H_
#define MAX 10000
char* soma (char op1[], char op2[]);
char* subtrair (char a[], char b[]);
char* completaMenor (char a[], char b[], char* menor);
char* multiplica (char a[], char b[]);
char* unsigneDiv (char a[], char D[], bool MOD);
char* unExpo (char a[], char b[]);
char* fatorial (char a[]);
int strIsDigit (char a[]);
int fatorial_multiplicador (int a, char fat[], int limit);
bool char2int (char a[]);
bool int2char (char a[], int tam);
bool inverte (char a[]); /* strrev (apenas para Janelas OS) */
int maior (int a, int b);
int menor (int a, int b);
bool strCmpNum (char x[], char b[]);
int numDigitos (int x);
void trataZeros (char** ptrNumber);

/*
**  GERENCIA MEMORIA ENTRE O RESULTADO E OS OPERADORES DAS FUNÇÕES
*/
bool memswap(char a[], char b[], char* (*f)(char*, char*));
bool memswapDiv(char a[], char b[], bool mod, char* (*f)(char*, char*, bool));
#endif
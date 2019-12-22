/*
 * =====================================================================================
 *
 *       Filename:  dict.c
 *
 *    Description: dict with bucket hash
 *
 *        Version:  1.0
 *        Created:  12/15/2019 11:20:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#include "dict.h"

#include <stdio.h>
#include <string.h>

const struct dict dicionario[] = {
    {"zero", "0"},
    {"um", "1"},
    {"dois", "2"},
    {"tres", "3"},
    {"quatro", "4"},
    {"cinco", "5"},
    {"seis", "6"},
    {"sete", "7"},
    {"oito", "8"},
    {"nove", "9"},
    {"dez", "10"},
    {"onze", "11"},
    {"doze", "12"},
    {"treze", "13"},
    {"catorze", "14"},
    {"quinze", "15"},
    {"dezesseis", "16"},
    {"dezessete", "17"},
    {"dezoito", "18"},
    {"dezenove", "19"},
    {"vinte", "20"},
    {"trinta", "30"},
    {"quarenta", "40"},
    {"cinquenta", "50"},
    {"sessenta", "60"},
    {"setenta", "70"},
    {"oitenta", "80"},
    {"noventa", "90"},
    {"cem", "100"},
    {"duzentos", "200"},
    {"trezentos", "300"},
    {"quatrocentos", "400"},
    {"quinhentos", "500"},
    {"seiscentos", "600"},
    {"setecentos", "700"},
    {"oitocentos", "800"},
    {"novecentos", "900"},
    {"mil", "1000"},
    {"milhao,milhoes", "1000000"},
    {"bilhao,bilhoes", "1000000000"},
    {"trilhao,trilhoes", "1000000000000"},
    {"quatrilhao,quatrilhoes", "1000000000000000"},
    {"quintilhao,quintilhoes", "1000000000000000000"},
    {"sextilhao,sextilhoes", "1000000000000000000000"},
    {"setilhao,setilhoes", "1000000000000000000000000"},
    {"octilhao,octilhoes", "1000000000000000000000000000"},
    {"nonilhao,nonilhoes", "1000000000000000000000000000000"},
    {"decilhao,decilhoes", "1000000000000000000000000000000000"},
    {"e", "e"},
    {"abre parentese", "("},
    {"fecha parentese", ")"},
    {"mais", "+"},
    {"menos", "-"},
    {"vezes", "*"},
    {"dividido por", "/"},
    {"mod", "%"},
    {"fatorial de", "!"},
    {"elevado a", "^"}};

BucketHash* dict = NULL;

BucketHash* initDict(void) {
   char key[MAXWLEN];
   BucketValue container;
   BucketValueDS valueDS = {{'\0'}, 0};
   unsigned len = sizeof dicionario / sizeof dicionario[0];
   BucketHash* h = bucketInit(len);
   unsigned i;

   container.data = (void*)&valueDS;
   container.capacity = sizeof valueDS;
   for (i = 0; i < len; i++) {
      strcpy(key, dicionario[i].key);
      strcpy(valueDS.valor, dicionario[i].value);
      char *pKey = key, *pComma = NULL;
      if ((pComma = strchr(pKey, ','))) {
         bucketPush(h, pComma + 1, &container);
         *pComma = '\0';
      }
      bucketPush(h, pKey, &container);
      valueDS.classe++;
   }
   return h;
}

/* int main(void) { */
/*    BucketHash* h = initDict(); */
/*    printf("length = %d\nsize = %d\n", h->length, h->size); */
/*    bucketFree(&h); */
/* } */

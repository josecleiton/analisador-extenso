/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/16/2019 08:51:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Cleiton Borges Correia (JCBC), jcleitonbc@gmail.com
 *   Organization:  UNEB
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

#define MAXWLEN 40

typedef struct value_classe {
   char valor[MAXWLEN];
   unsigned classe;
} BucketValueDS;

BucketHash* initDict(void) {
   FILE* f = fopen("./lib/dicionario.cfg", "rt");
   char key[MAXWLEN], line[128];
   BucketValue container;
   BucketValueDS valueDS;
   int len = 0;
   while (fgets(line, 128, f)) len++;
   rewind(f);
   BucketHash* h = bucketInit(len);
   for (valueDS.classe = 0;
        fscanf(f, "%[^=]=%[^\n]%*c", key, valueDS.valor) != EOF;
        valueDS.classe++) {
      char *pKey = key, *pComma = NULL;
      container.data = (void*)&valueDS;
      container.capacity = (strlen(valueDS.valor) + 1) + sizeof valueDS.classe;
      if ((pComma = strchr(pKey, ','))) {
         bucketPush(h, pComma + 1, &container);
         *pComma = '\0';
      }
      bucketPush(h, pKey, &container);
   }
   /* puts(bucketFind(h, "decilhoes")); */
   fclose(f);
   return h;
}

int main() {
   BucketHash* d = initDict();
   printf("length: %d\nsize: %d\n", d->length, d->size);
   for (unsigned i = 0; i < d->length; i++) {
      if (!d->heads[i]) {
         printf("head %d is empty\n", i);
      }
   }
   bucketFree(&d);
   return 0;
}

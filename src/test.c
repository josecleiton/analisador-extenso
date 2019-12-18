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

BucketHash* initDict(void) {
   FILE* f = fopen("./lib/dicionario.cfg", "rt");
   char key[MAXWLEN], value[MAXWLEN], line[128];
   int len = 0;
   while(fgets(line, 128, f)) len++;
   rewind(f);
   BucketHash *h = bucketInit(len);
   while(fscanf(f, "%[^=]=%[^\n]%*c", key, value) != EOF) {
      char *pKey = key, *pValue = value, *comma = NULL;
      if((comma = strchr(pKey, ','))) {
         bucketPush(h, comma + 1, pValue);
         *comma = '\0';
      }
      bucketPush(h, pKey, pValue);
   }
   puts(bucketFind(h, "decilhoes"));
   puts(bucketFind(h, "decilhao"));
   if(bucketFind(h, "fatorial")) {
      puts("QUE VIAGE");
   }
   puts(bucketFind(h, "fatorial-de"));
   fclose(f);
   return h;
}


int main() {
   BucketHash* d = initDict();
   bucketFree(&d);
   return 0;
}

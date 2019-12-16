#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alloc.h"
#include "hash.h"

unsigned long hash(const char* in) {
   unsigned long hash = 5381;
   char* str = (char*) in;
   int c;
   while(c = *str++) {
      hash = hash * 33 + c;
   }
   return hash;
}


BucketHash* bucketInit(const int length) {
   BucketHash* h = (BucketHash*) alloc(1, sizeof(BucketHash));
   h->heads = (BucketList**) alloc(length, sizeof(BucketList*));
   h->length = length;
   return h;
}

int bucketPush(BucketHash *h, const char* str) {
   int idx = hash(str) % h->length;
   h->length += bucketListPush(&h->heads[idx], str);
}

int bucketListPush(BucketList** l, const char* str) {
   BucketList* list = *l;
   if(list) {
      return bucketListPush(&list->next, str) + 1;
   }
   list = *l = (BucketList*) alloc(1, sizeof(BucketList));
   list->key = (char*) alloc(strlen(str), sizeof(char));
   strcpy(list->key, str);
   return 1;
}

int bucketPop(BucketHash *h, const char* str) {
   int idx = hash(str) % h->length;
   return bucketListPop(&h->heads[idx], str);
}

int bucketListPop(BucketList** l, const char* str) {
   BucketList *list = *l;
   if(!strcmp(list->key, str)) {
      // achou
      
   }
}


#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"

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
   bucketListPush(&h->heads[idx], str);
}

int bucketListPush(BucketList** l, const char* str) {
   BucketList* list = *l;
   while(list && list->next) {
      list = list->next;
   }
   if(list) {
      list->next = bucketListAlloc(str);
   } else {
      *l = bucketListAlloc(str);
   }
   return ((*l)->length++);
}


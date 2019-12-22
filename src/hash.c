#include "hash.h"

#include <stdlib.h>
#include <string.h>

#include "alloc.h"

unsigned long hash(const char* in) {
   unsigned long hash = 5381;
   char* str = (char*)in;
   unsigned c;
   while ((c = *str++)) {
      /* hash * 33 + c */
      hash = ((hash << 5) + hash) + c;
   }
   return hash;
}

BucketHash* bucketInit(const int length) {
   BucketHash* h = (BucketHash*)alloc(1, sizeof(BucketHash));
   h->heads = (BucketList**)alloc(length, sizeof(BucketList*));
   h->length = length;
   return h;
}

int bucketPush(BucketHash* h, const char* key, const BucketValue* v) {
   int idx = hash(key) % h->length;
   return (h->size += bucketListPush(&h->heads[idx], key, v));
}

int bucketListPush(BucketList** l, const char* key, const BucketValue* value) {
   BucketList* list = *l;
   if (list) {
      if (!strcmp(list->key, key)) {
         bucketListValueAssign(&list->value, value);
         return 0;
      }
      return bucketListPush(&list->next, key, value);
   }
   list = *l = (BucketList*)alloc(1, sizeof(BucketList));
   list->key = (char*)alloc(strlen(key) + 1, sizeof(char));
   bucketListValueAssign(&list->value, value);
   strcpy(list->key, key);
   return 1;
}

void bucketListValueAssign(BucketValue* lv, const BucketValue* rv) {
   const unsigned capacity = rv->capacity;
   if (lv->capacity) {
      if (lv->capacity < capacity) {
         lv->capacity = capacity << 1;
         lv->data = realloc(lv->data, lv->capacity);
         if (!lv->data) {
            abortWithLog(true);
         }
      }
      memcpy(lv->data, rv->data, lv->capacity);
   } else {
      lv->data = alloc(capacity, 1);
      lv->capacity = capacity;
      memcpy(lv->data, rv->data, capacity);
   }
}

int bucketPop(BucketHash* h, const char* key) {
   int idx = hash(key) % h->length;
   int removed = 0;
   h->heads[idx] = bucketListPop(&h->heads[idx], key, &removed);
   return (h->size -= removed);
}

BucketList* bucketListPop(BucketList** l, const char* key, int* removed) {
   BucketList* list = *l;
   if (list) {
      if (!strcmp(list->key, key)) {
         /* achou */
         BucketList* nextPtr = list->next;
         bucketNodeListFree(list);
         *removed = 1;
         return nextPtr;
      } else {
         list->next = bucketListPop(&list->next, key, removed);
         return list;
      }
   }
   return NULL;
}

void bucketFree(BucketHash** h) {
   BucketHash* hash = *h;
   for (unsigned i = 0; i < hash->length; i++) {
      hash->heads[i] = bucketListFree(hash->heads[i]);
   }
   free(hash->heads);
   free(hash);
   *h = NULL;
}

BucketList* bucketListFree(BucketList* l) {
   if (l) {
      l->next = bucketListFree(l->next);
      bucketNodeListFree(l);
   }
   return NULL;
}

const BucketValue* bucketFind(BucketHash* h, const char* key) {
   int idx = hash(key) % h->length;
   return bucketListFind(h->heads[idx], key);
}

const BucketValue* bucketListFind(BucketList* list, const char* key) {
   if (list) {
      if (!strcmp(list->key, key)) {
         return &list->value;
      }
      return bucketListFind(list->next, key);
   }
   return NULL;
}

void bucketNodeListFree(BucketList* l) {
   free(l->key);
   free(l->value.data);
   free(l);
}

/* int main() { */
/*    BucketHash* h = bucketInit(10); */
/*    BucketValue v; */
/*    v.data = (void*)"test"; */
/*    v.capacity = 6; */
/*    bucketPush(h, "test", &v); */
/*    v.data = (void*)"treme"; */
/*    v.capacity = 6; */
/*    bucketPush(h, "italo", &v); */
/*    v.data = (void*)"burra"; */
/*    v.capacity = 6; */
/*    bucketPush(h, "Rita", &v); */
/*    v.data = (void*)"inteligente"; */
/*    v.capacity = 12; */
/*    bucketPush(h, "milena", &v); */
/*    v.data = (void*)"burra"; */
/*    v.capacity = 6; */
/*    bucketPush(h, "milena", &v); */
/*    bucketPop(h, "Rita"); */
/*    bucketPop(h, "italo sergio"); */
/*    /1* bucketPop(h, "milena"); *1/ */
/*    const BucketValue* r = bucketFind(h, "milena"); */
/*    if (r) { */
/*       printf("milena = %s\n", (const char*)r->data); */
/*    } else { */
/*       puts("milena não está no dat"); */
/*    } */
/*    bucketFree(&h); */
/* } */

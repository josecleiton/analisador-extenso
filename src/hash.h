#ifndef HASH_H
#define HASH_H

typedef struct bucket_value {
   void* data;
   unsigned capacity;
} BucketValue;

typedef struct bucket_list {
   char* key;
   BucketValue value;
   struct bucket_list* next;
} BucketList;

typedef struct bucket_hash {
   BucketList** heads;
   unsigned length;  // total de buckets alocados
   unsigned size;    // total de nós em todos os buckets
} BucketHash;

/*
 * DJB2 hash function
 */
unsigned long hash(const char* in);

/*
 * Inicializa ponteiros para k = length
 */
BucketHash* bucketInit(const int length);

/*
 * Insere par de chave valor
 */
int bucketPush(BucketHash* h, const char* key, const BucketValue* value);

/*
 * Remove chave
 */
int bucketPop(BucketHash* h, const char* key);

/*
 * Busca chave
 */
const BucketValue* bucketFind(BucketHash* h, const char* key);

/*
 * Libera memória de toda a estrutura de dados
 * Para evitar memory leak,
 * deve ser chamada no fim da utilização
 */
void bucketFree(BucketHash**);

/*
 * Abaixo há funções auxiliares para
 * manipular as listas (buckets),
 * assim como seus nós
 */

int bucketListPush(BucketList** list, const char* key, const BucketValue* v);

void bucketListValueAssign(BucketValue* toAssign, const BucketValue* value);

BucketList* bucketListPop(BucketList** list, const char* key, int* removed);

BucketList* bucketListFree(BucketList* list);

const BucketValue* bucketListFind(BucketList* list, const char* key);

void bucketNodeListFree(BucketList*);

#endif

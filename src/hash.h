typedef struct bucket_list {
   struct bucket_list* next;
   unsigned length;
   char *key, *value;
   int vcapacity;  // value capacity
} BucketList;

typedef struct bucket_hash {
   BucketList** heads;
   unsigned length, size;
} BucketHash;

unsigned long hash(const char* in);

BucketHash* bucketInit(const int length);

int bucketPush(BucketHash* h, const char* key, const char* value);

int bucketPop(BucketHash* h, const char* key);

const char* bucketFind(BucketHash* h, const char* key);

void bucketFree(BucketHash**);

int bucketListPush(BucketList** list, const char* str, const char* value,
                   int* pushed);

BucketList* bucketListPop(BucketList** list, const char* str, int* removed);

BucketList* bucketListFree(BucketList* list);

BucketList* bucketListAlloc(const char* str);

const char* bucketListFind(BucketList* list, const char* key);


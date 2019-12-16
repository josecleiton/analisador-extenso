typedef struct bucket_list {
   struct bucket_list* next;
   int length;
   char* key;
} BucketList;

typedef struct bucket_hash {
   BucketList** heads;
   int length;
} BucketHash;

unsigned long hash(const char *in);

BucketHash* bucketInit(const int length);

int bucketPush(BucketHash* h, const char* str);

int bucketPop(BucketHash* h, const char* key);

int bucketFind(BucketHash* h, const char* key);

int bucketUpdate(BucketHash* h, const char* oldK, const char* newK);

void bucketFree(BucketHash**);

int bucketListPush(BucketList** list, const char* str);

int bucketListPop(BucketList** list, const char* str);

BucketList* bucketListAlloc(const char* str);


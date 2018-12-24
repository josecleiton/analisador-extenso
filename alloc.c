#include "alloc.h"

void* alloc (size_t count, size_t blockSize)
{
    void* ptr = calloc(count, blockSize);
    if(! ptr) abortWithLog(true);
    return ptr;
}

FILE* openFile (const char file_name[], const char type[])
{
    FILE* ptr_file = fopen(file_name, type);
    if(! ptr_file)
        abortWithLog(true);
    return ptr_file;
}

int abortWithLog (bool msg)
{
    if(msg) fprintf (stderr, "Um erro ocorreu na execução do programa: \n");
    fprintf (stderr, "%s", strerror(errno));
    abort ();
}

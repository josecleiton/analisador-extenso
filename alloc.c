#include "alloc.h"

void* alloc (const size_t count, const size_t blockSize)
{
    void* ptr = calloc(count, blockSize);
    if(! ptr) abortWithLog(true);
    return ptr;
}

FILE* openFile (const char filename[], const char type[])
{
    FILE* fileptr = fopen(filename, type);
    if(! fileptr)
        abortWithLog(true);
    return fileptr;
}

int abortWithLog (const bool msg)
{
    if(msg) fprintf (stderr, "Um erro ocorreu na execução do programa: \n");
    fprintf (stderr, "%s\n", strerror(errno));
    abort ();
}

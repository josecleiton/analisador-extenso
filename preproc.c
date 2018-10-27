#include "preproc.h"

void* MALLOC (size_t size)
{
    void* ptr;
    ptr = malloc (size);
    if (!ptr)
    {
        fprintf (stderr, "Memoria insuficiente.\n");
        ERRO;
    }
    memset(ptr, 0, size);
    return ptr;
}
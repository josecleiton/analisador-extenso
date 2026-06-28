#include "extenso/util.h"
#include "extenso/context.h"

void *
arenaAlloc (Context *ctx, size_t n)
{
    void *p = alloc (n, 1);
    if (ctx->arena_count == ctx->arena_cap)
        {
            ctx->arena_cap = ctx->arena_cap ? ctx->arena_cap * 2 : 16;
            ctx->arena = (void **)realloc (ctx->arena, ctx->arena_cap * sizeof (void *));
            if (!ctx->arena)
                abortWithLog (true);
        }
    ctx->arena[ctx->arena_count++] = p;
    return p;
}

void
arenaReset (Context *ctx)
{
    for (size_t i = 0; i < ctx->arena_count; i++)
        free (ctx->arena[i]);
    ctx->arena_count = 0;
}

void
arenaFree (Context *ctx)
{
    arenaReset (ctx);
    free (ctx->arena);
    ctx->arena = NULL;
    ctx->arena_cap = 0;
}

void *
alloc (const size_t count, const size_t blockSize)
{
    void *ptr = calloc (count, blockSize);
    if (!ptr)
        abortWithLog (true);
    return ptr;
}

FILE *
openFile (const char filename[], const char type[])
{
    FILE *fileptr = fopen (filename, type);
    if (!fileptr)
        abortWithLog (true);
    return fileptr;
}

int
abortWithLog (const bool msg)
{
    if (msg)
        fprintf (stderr, "Um erro ocorreu na execução do programa: \n");
    fprintf (stderr, "%s\n", strerror (errno));
    abort ();
}

Index
buildLineIndex (FILE *in, int limite)
{
    Index result;
    char handle[MAX_GEN];
    bool rlloc = false;
    if (!limite)
        {
            rlloc = true;
            limite = 32;
        }
    int i = 0;
    uint16_t *index = (uint16_t *)alloc (limite, sizeof (uint16_t));
    rewind (in);
    index[i++] = ftell (in);
    while (fgets (handle, MAX_GEN, in))
        {
            index[i++] = ftell (in);
            if (i == limite && rlloc)
                {
                    limite <<= 1;
                    index = (uint16_t *)realloc (index, sizeof (uint16_t) * limite);
                    if (!index)
                        abortWithLog (true);
                }
        }
    index[--i] = 0;
    rewind (in);
    result.index = index;
    result.tam = i;
    return result;
}

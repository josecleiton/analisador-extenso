#ifndef EXTENSO_UTIL_H
#define EXTENSO_UTIL_H

/*
**  Utilities: allocation with error handling, file opening and file line
**  indexing.
*/
#include "extenso/config.h"
#include "extenso/types.h"

/* calloc that aborts on failure. */
void *alloc (const size_t count, const size_t blockSize);

/* fopen that aborts on failure. */
FILE *openFile (const char filename[], const char type[]);

/* Reports errno and aborts the program (unrecoverable failure). */
int abortWithLog (const bool msg);

/* Returns the positions (offsets) of the start of each line of `in`. */
Index buildLineIndex (FILE *in, int limite);

#endif

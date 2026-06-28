#ifndef EXTENSO_ERRORS_H
#define EXTENSO_ERRORS_H

/*
**  Error message table loaded into memory (see ARQ_ERROS) and reporting of
**  lexical/syntactic/semantic errors.
*/
#include "extenso/config.h"

struct Context; /* forward decl (avoids a cycle with context.h) */

typedef struct
{
    char **mensagens;
    size_t n; /* number of messages (was NUM_ERROS) */
} ErrorTable;

/* Loads the table; `n` derived from the line count. */
ErrorTable *error_table_load (const char *path);

/* Frees the table. */
void error_table_free (ErrorTable *t);

/* Message for error `tipo` (with bounds checking). */
const char *error_message (const ErrorTable *t, int tipo);

/* Formats error `tipoErro`, logs it and aborts. */
void reportError (struct Context *ctx, int tipoErro);

#endif

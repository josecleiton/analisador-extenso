#ifndef EXTENSO_CONTEXT_H
#define EXTENSO_CONTEXT_H

/*
**  Context: state of an analysis, threaded through the lexer/parser/semantics/
**  conversion. Replaces the old global variables. The `cli` owns one Context
**  per run.
**
**  (The field names mirror the old globals; they will be translated to English
**  in Phase 8.)
*/
#include "extenso/config.h"
#include "extenso/dictionary.h"
#include "extenso/errors.h"
#include "extenso/types.h"
#include <setjmp.h>

typedef struct Context
{
    char buffer[MAX_GEN]; /* buffer of the expression being analyzed */
    char *cursor;         /* movable cursor over buffer */
    char *exprStart;      /* start of the expression, for the error caret */
    char token;           /* current token */
    short tokenType;      /* type of the current token */
    unsigned isNumber;    /* is the token(s) being analyzed a number? */
    NumList *list;        /* number analyzed digit by digit, in a linked list */

    const Dictionary *dict;   /* dictionary in memory (shared) */
    const ErrorTable *errtab; /* error table in memory (shared) */

    jmp_buf on_error;     /* longjmp target on expression error */
    bool error_protected; /* is there an active setjmp handler? */
} Context;

#endif

#ifndef EXTENSO_DICTIONARY_H
#define EXTENSO_DICTIONARY_H

/*
**  Dictionary loaded into memory just once (replaces the fseek/fscanf access
**  to ARQ_DICT on every token).
**
**  Each entry holds the raw line from the .cfg: `name` is everything before
**  the '=' (it may contain the compound form "singular,plural", e.g.
**  "milhao,milhoes") and `value` is everything after the '='. The entry index
**  is exactly the value of the corresponding `tokens` enum.
*/
#include "extenso/config.h"

typedef struct
{
    char name[MAXWLEN];
    char value[MAXWLEN];
} DictEntry;

typedef struct
{
    DictEntry *items;
    size_t n;           /* number of entries (was TAM_DICT) */
    size_t delim_start; /* index of the 1st delimiter entry (was INDEL) */
} Dictionary;

/* Loads the dictionary; `n`/`delim_start` derived from the content. */
Dictionary *dictionary_load (const char *path);

/* Frees the dictionary. */
void dictionary_free (Dictionary *d);

#endif

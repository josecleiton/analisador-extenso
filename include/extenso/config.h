#ifndef EXTENSO_CONFIG_H
#define EXTENSO_CONFIG_H

/*
**  System includes and shared configuration macros.
*/
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Data files (relative to the working directory). */
#define ARQ_DICT "lib/dicionario.cfg"    /* <word>=<value> per line */
#define ARQ_ERROS "lib/erros.cfg"        /* one error type per line */
#define ARQ_ENTRADA "lib/expressoes.txt" /* expressions to analyze */
#define ARQ_SAIDA "resultados.txt"       /* result of the expressions */
#define ARQ_LOG "logs.txt"               /* date + description of each error */

/* STR_DEL: delimiter characters. */
#define STR_DEL "+/%-*!e()^"

/* MAX_GEN: generic string size. */
#define MAX_GEN 1024

/* MAXWLEN: maximum length of the words in ARQ_DICT (before and after the '='). */
#define MAXWLEN 35

/* _1KB: 2^10 bytes. */
#define _1KB 1024

/* CLRBUF: clears the input buffer. */
#define CLRBUF scanf ("%*c")

#endif

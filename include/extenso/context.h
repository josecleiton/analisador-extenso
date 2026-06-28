#ifndef EXTENSO_CONTEXT_H
#define EXTENSO_CONTEXT_H

/*
**  Context: estado de uma análise, encadeado pelo lexer/parser/semântica/
**  conversão. Substitui as antigas variáveis globais. O `cli` é dono de um
**  Context por execução.
**
**  (Os nomes dos campos espelham as antigas globais; serão traduzidos para
**  inglês na Fase 8.)
*/
#include "extenso/config.h"
#include "extenso/dictionary.h"
#include "extenso/errors.h"
#include "extenso/types.h"
#include <setjmp.h>

typedef struct Context
{
    char buffer[MAX_GEN]; /* buffer da expressão em análise */
    char *cursor;         /* cursor móvel sobre buffer */
    char *exprStart;      /* início da expressão, para o caret de erro */
    char token;           /* token corrente */
    short tokenType;      /* tipo do token corrente */
    unsigned isNumber;    /* token(s) em análise são número? */
    NumList *list;        /* número analisado por casas, em lista encadeada */

    const Dictionary *dict;   /* dicionário em memória (compartilhado) */
    const ErrorTable *errtab; /* tabela de erros em memória (compartilhada) */

    jmp_buf on_error;     /* destino do longjmp em erro de expressão */
    bool error_protected; /* há um handler setjmp ativo? */
} Context;

#endif

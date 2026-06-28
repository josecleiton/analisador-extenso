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
#include "extenso/types.h"
#include "extenso/dictionary.h"
#include "extenso/errors.h"

typedef struct Context
{
    char expNum[MAX_GEN]; /* buffer da expressão em análise */
    char *EXP;            /* cursor móvel sobre expNum */
    char *_TEXP;          /* início da expressão, para o caret de erro */
    char token;           /* token corrente */
    short tipoToken;      /* tipo do token corrente */
    unsigned flagNUM;     /* token(s) em análise são número? */
    ListaNum *list;       /* número analisado por casas, em lista encadeada */

    const Dictionary *dict;   /* dicionário em memória (compartilhado) */
    const ErrorTable *errtab; /* tabela de erros em memória (compartilhada) */
} Context;

#endif

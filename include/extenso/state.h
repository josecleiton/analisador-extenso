#ifndef EXTENSO_STATE_H
#define EXTENSO_STATE_H

/*
**  Estado global compartilhado entre lexer/parser/semântica/conversão.
**
**  TEMPORÁRIO: existe apenas para manter o split com comportamento idêntico.
**  Será substituído por uma struct Context encadeada na Fase 4.
**  Definições em src/state.c.
*/
#include "extenso/config.h"
#include "extenso/types.h"
#include "extenso/dictionary.h"
#include "extenso/errors.h"

extern char *EXP;            /* cursor móvel sobre expNum */
extern char *_TEXP;          /* expressão original, para exibição de erros */
extern char expNum[MAX_GEN]; /* expressão em análise */
extern char token;           /* token corrente */
extern short tipoToken;      /* tipo do token corrente */
extern unsigned flagNUM;     /* token(s) em análise são número? */
extern ListaNum *list;       /* número analisado por casas, em lista encadeada */

extern const Dictionary *dict;  /* dicionário carregado em memória */
extern const ErrorTable *errtab; /* tabela de erros em memória */

#endif

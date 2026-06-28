#ifndef EXTENSO_STATE_H
#define EXTENSO_STATE_H

/*
**  Estado global compartilhado entre lexer/parser/semântica/conversão.
**
**  TEMPORÁRIO: existe apenas para manter o split da Fase 2 com comportamento
**  idêntico. Será substituído por uma struct Context encadeada na Fase 4.
**  Definições em src/state.c.
*/
#include "extenso/config.h"
#include "extenso/types.h"

extern Ordem ref;          /* scratch de uma linha do ARQ_DICT */
extern char *EXP;          /* cursor móvel sobre expNum */
extern char *_TEXP;        /* expressão original, para exibição de erros */
extern char expNum[MAX_GEN]; /* expressão em análise */
extern char token;         /* token corrente */
extern short tipoToken;    /* tipo do token corrente */
extern unsigned flagNUM;   /* token(s) em análise são número? */
extern uint16_t *ind;      /* posições das linhas do ARQ_DICT */
extern FILE *dicionario;   /* arquivo de tokens (dicionário) */
extern ListaNum *list;     /* número analisado por casas, em lista encadeada */

#endif

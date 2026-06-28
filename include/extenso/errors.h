#ifndef EXTENSO_ERRORS_H
#define EXTENSO_ERRORS_H

/*
**  Tabela de mensagens de erro carregada em memória (ver ARQ_ERROS) e
**  relatório de erros léxicos/sintáticos/semânticos.
*/
#include "extenso/config.h"

struct Context; /* forward decl (evita ciclo com context.h) */

typedef struct
{
    char **mensagens;
    size_t n; /* nº de mensagens (era NUM_ERROS) */
} ErrorTable;

/* Carrega a tabela; `n` derivado da contagem de linhas. */
ErrorTable *error_table_load (const char *path);

/* Libera a tabela. */
void error_table_free (ErrorTable *t);

/* Mensagem do erro `tipo` (com checagem de limites). */
const char *error_message (const ErrorTable *t, int tipo);

/* Formata o erro `tipoErro`, registra em log e aborta. */
void reportError (struct Context *ctx, int tipoErro);

#endif

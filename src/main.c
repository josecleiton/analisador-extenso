/*
**  Universidade Estadual do Estado da Bahia - UNEB
**  Por: José Cleiton Borges Correia
**  Prof: Jorge Sampaio Farias
**  Disciplina: Linguagem de Programação 1 - 2018.1
**  #################################################
**  | AVALIADOR DE EXPRESSÕES NUMERICAS POR EXTENSO |
**  | Entrada: expressão numérica por extenso       |
**  | Saída: expressão resolvida por extenso        |
**  #################################################
*/
#include <string.h>
#include "extenso/cli.h"
#include "extenso/config.h"
#include "extenso/context.h"
#include "extenso/dictionary.h"
#include "extenso/errors.h"
#include "extenso/parser.h"

int main (int argc, char **argv)
{
    Dictionary *d = dictionary_load (ARQ_DICT);
    ErrorTable *e = error_table_load (ARQ_ERROS);

    Context ctx = {0};
    ctx.dict = d;
    ctx.errtab = e;

    int rc;
    /* Modo batch não-interativo (usado pelo teste de regressão):
    ** lê ARQ_ENTRADA, escreve ARQ_SAIDA e sai, sem o menu. */
    if (argc > 1 && strcmp (argv[1], "--batch") == 0)
    {
        fileParsingInit (&ctx);
        rc = 0;
    }
    /* Avalia uma única expressão e imprime só o resultado. */
    else if (argc > 2 && strcmp (argv[1], "--eval") == 0)
    {
        ctx.EXP = ctx.expNum;
        strncpy (ctx.expNum, argv[2], MAX_GEN - 1);
        ctx.expNum[MAX_GEN - 1] = '\0';
        char *r = expParsingStart (&ctx);
        printf ("%s\n", r);
        free (ctx._TEXP);
        rc = 0;
    }
    else rc = interpretador (&ctx);

    dictionary_free (d);
    error_table_free (e);
    return rc;
}

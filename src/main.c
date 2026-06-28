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
        const char *inPath  = argc > 2 ? argv[2] : ARQ_ENTRADA;
        const char *outPath = argc > 3 ? argv[3] : ARQ_SAIDA;
        runFile (&ctx, inPath, outPath);
        rc = 0;
    }
    /* Avalia uma única expressão e imprime só o result. */
    else if (argc > 2 && strcmp (argv[1], "--eval") == 0)
    {
        ctx.cursor = ctx.buffer;
        strncpy (ctx.buffer, argv[2], MAX_GEN - 1);
        ctx.buffer[MAX_GEN - 1] = '\0';
        ctx.error_protected = true;
        if (setjmp (ctx.on_error) == 0)
        {
            char *r = evalExpr (&ctx);
            printf ("%s\n", r);
            free (ctx.exprStart);
            rc = 0;
        }
        else rc = 1; /* expressão inválida: erro já reportado por reportError */
    }
    else rc = runMenu (&ctx);

    dictionary_free (d);
    error_table_free (e);
    return rc;
}

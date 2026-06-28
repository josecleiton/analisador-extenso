#include "extenso/errors.h"
#include "extenso/config.h"
#include "extenso/context.h"
#include "extenso/util.h"
#include <time.h>

ErrorTable *
error_table_load (const char *path)
{
    FILE *f = openFile (path, "r");
    size_t cap = 16, n = 0;
    char **msgs = (char **)alloc (cap, sizeof (char *));
    char line[MAX_GEN];
    while (fgets (line, sizeof line, f))
        {
            line[strcspn (line, "\r\n")] = '\0';
            if (n == cap)
                {
                    cap <<= 1;
                    msgs = (char **)realloc (msgs, cap * sizeof (char *));
                    if (!msgs)
                        abortWithLog (true);
                }
            msgs[n] = (char *)alloc (strlen (line) + 1, sizeof (char));
            strcpy (msgs[n], line);
            n++;
        }
    fclose (f);

    ErrorTable *t = (ErrorTable *)alloc (1, sizeof (ErrorTable));
    t->mensagens = msgs;
    t->n = n;
    return t;
}

void
error_table_free (ErrorTable *t)
{
    if (!t)
        return;
    for (size_t i = 0; i < t->n; i++)
        free (t->mensagens[i]);
    free (t->mensagens);
    free (t);
}

const char *
error_message (const ErrorTable *t, int tipo)
{
    if (tipo < 0 || (size_t)tipo >= t->n)
        return "erro desconhecido";
    return t->mensagens[tipo];
}

void
reportError (Context *ctx, int tipoErro)
{
    char strErro[MAX_GEN], *strBump;
    int temp, i = 0, tamErro, tamEXP;
    strcpy (strErro, error_message (ctx->errtab, tipoErro));
    strcat (strErro, "\n\n\t");
    strcat (strErro, ctx->exprStart);
    strcat (strErro, "\n\t");
    temp = ctx->cursor - ctx->exprStart;
    tamErro = strlen (strErro);
    while (i < temp)
        {
            strErro[tamErro + i] = ' ';
            i++;
        }
    strErro[tamErro + i] = '^';
    strBump = &strErro[tamErro + i + 1];
    for (tamEXP = strlen (ctx->exprStart); tamEXP - temp; tamEXP--)
        *strBump++ = '~';
    *strBump++ = '\n';
    *strBump++ = '\0';
    char *toFile;
    uint16_t size_toFile = strlen (strErro) + 50;
    time_t now;
    struct tm *timeinfo;
    time (&now);
    timeinfo = localtime (&now);
    toFile = (char *)alloc (size_toFile, sizeof (char));
    strcpy (toFile, asctime (timeinfo));
    char *needle = strchr (toFile, '\n');
    *++needle = '\0';
    strcat (toFile, strErro);
    needle = strrchr (toFile, '\n');
    *++needle = '\0';
    FILE *logs = openFile (ARQ_LOG, "at");
    fputs (toFile, logs);
    fflush (logs);
    fclose (logs);
    free (toFile);
    puts (strErro);
    /* Erro de entrada do usuário: volta ao handler por-expressão (o programa
    ** segue para a próxima expressão) em vez de abortar. Se não houver handler
    ** ativo, aborta como antes. */
    if (ctx->error_protected)
        longjmp (ctx->on_error, tipoErro + 1);
    abortWithLog (false);
}

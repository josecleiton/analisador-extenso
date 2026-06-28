#include "extenso/lexer.h"
#include "extenso/tokens.h"
#include "extenso/num_list.h"
#include "extenso/errors.h"

void nextToken (Context *ctx)
{
    Term ref;
    int i = 0, k = 0;
    char trade = '\0';
    char valorTk = '\0';
    ctx->token = '\0';
    ctx->tokenType = 0;
    if (!*ctx->cursor) return;
    while (isspace (*ctx->cursor)) ++ctx->cursor;
    while (ctx->cursor[k] && isalpha (ctx->cursor[k])) k++;
    trade = ctx->cursor[k];
    ctx->cursor[k] = '\0';
    joinCompoundDelim (ctx, &k, &trade);
    while (i < (int) ctx->dict->n)
    {
        strcpy (ref.name, ctx->dict->items[i].name);
        strcpy (ref.value, ctx->dict->items[i].value);
        if (! strcmp (ref.name, ctx->cursor) || matchPlural (ctx, i, ref.name))
        {
            valorTk = *(ref.value);
            if (isdigit (valorTk))
            {
                ctx->token = valorTk;
                while (*ctx->cursor && (isalpha (*ctx->cursor) || *ctx->cursor == ' ')) ctx->cursor++;
                *ctx->cursor = trade;
                ctx->tokenType = NUM;
                ctx->isNumber = true;
                listAppend (ctx, i, ref.name, ref.value);
                i = -1;
                if (nextIsDelimiter (ctx)) return;
            }
            else if (strchr (STR_DEL, valorTk))
            {
                ctx->tokenType = CONJUCAO;
                while (*ctx->cursor && (isalpha (*ctx->cursor) || *ctx->cursor == ' ' || *ctx->cursor == '-')) ctx->cursor++;
                ctx->token = valorTk;
                *ctx->cursor = trade;
                if (i != CONJUCAO)
                {
                    ctx->tokenType = DELIMITADOR;
                    ctx->isNumber = false;
                    return;
                }
                else
                {
                    listAppend (ctx, i, ref.name, ref.value);
                    i = -1;
                }
            }
        }
        else skipWord (ctx);
        i++;
    }
    reportError (ctx, 0);
}

void skipWord (Context *ctx)
{
    while (*ctx->cursor && *ctx->cursor == ' ') ctx->cursor++;
    int k = strcspn (ctx->cursor, (const char*) " ");
    ctx->cursor[k] = '\0';
}

bool nextIsDelimiter (Context *ctx)
{
    while (*ctx->cursor && *ctx->cursor == ' ') ctx->cursor++; /* Posiciona a analise no inicio do proximo token */
    char* needle = strchr (ctx->cursor, ' ');
    if (! needle)
    {
        if (*ctx->cursor) return 0;
        return 1;
    }
    int k = needle - ctx->cursor;
    ctx->cursor[k] = '\0';
    char DEL[MAXWLEN] = {'\0'};
    size_t j = ctx->dict->delim_start + 1; /* PULA O 'e' (PRIMEIRO DELIMITADOR) */
    while (j < ctx->dict->n)
    {
        strcpy (DEL, ctx->dict->items[j++].name);
        char* needle2 = strchr (DEL, '-'); /* TRATA O HIFEN NO DELIMITADOR COMPOSTO */
        if (needle2)
            *needle2 = '\0';
        if (!strcmp (DEL, ctx->cursor))
        {
            ctx->cursor[k] = ' ';
            return 1;
        }
    }
    ctx->cursor[k] = ' ';
    return 0;
}

bool matchPlural (Context *ctx, int i, char *currentToken)
{
    (void) i;
    if (! strchr ("mbtqdscount", *currentToken)) return 0;
    char* del = strchr (currentToken, ',');
    bool fl = false;
    if (!del) return 0;
    int k = del - currentToken;
    currentToken[k] = '\0';

    if (! strcmp (currentToken, ctx->cursor))
        fl = true;
    else
    {
        ++del;
        if (! strcmp (del, ctx->cursor))
        {
            char temp[MAXWLEN] = {'\0'};
            fl = true;
            strcpy (temp, del);
            strcpy (currentToken, temp);
            currentToken[++k] = '\0';
        }
    }
    return fl;
}

void joinCompoundDelim (Context *ctx, int* k, char* temp)
{
    if (*ctx->cursor != 'a' && *ctx->cursor != 'f' && *ctx->cursor != 'd' && *ctx->cursor != 'e') return;
    else if (! strcmp (ctx->cursor, (const char*) "abre") || ! strcmp (ctx->cursor, (const char*) "fecha") || \
             ! strcmp (ctx->cursor, (const char*) "dividido") || ! strcmp (ctx->cursor, (const char*) "fatorial") || \
             ! strcmp (ctx->cursor, (const char*) "elevado") \
            )
    {
        int i = 0;
        ctx->cursor[*k] = '-';
        while (isalpha(ctx->cursor[i]) || ctx->cursor[i] == '-') i++;
        *temp = ctx->cursor[i];
        ctx->cursor[i] = '\0';
        if (strcmp (&ctx->cursor[*k+1], (const char*)"parentese") && strcmp (&ctx->cursor[*k+1], (const char*) "de") && \
            strcmp (&ctx->cursor[*k+1], (const char*) "por") && strcmp(&ctx->cursor[*k+1], (const char*) "a")
            )
        {
            ctx->cursor[*k] = ' ';
            reportError (ctx, 0);
            return;
        }
    }

}

void lowercaseExpr (Context *ctx)
{
    int i = 0;
    while(ctx->cursor[i]){
        ctx->cursor[i] = tolower (ctx->cursor[i]);
        i++;
    }
}

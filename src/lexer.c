#include "extenso/lexer.h"
#include "extenso/tokens.h"
#include "extenso/num_list.h"
#include "extenso/errors.h"

void pegaToken (Context *ctx)
{
    Ordem ref;
    int i = 0, k = 0;
    char trade = '\0';
    char valorTk = '\0';
    ctx->token = '\0';
    ctx->tipoToken = 0;
    if (!*ctx->EXP) return;
    while (isspace (*ctx->EXP)) ++ctx->EXP;
    while (ctx->EXP[k] && isalpha (ctx->EXP[k])) k++;
    trade = ctx->EXP[k];
    ctx->EXP[k] = '\0';
    ajustaDelim (ctx, &k, &trade);
    while (i < (int) ctx->dict->n)
    {
        strcpy (ref.nome, ctx->dict->items[i].nome);
        strcpy (ref.valor, ctx->dict->items[i].valor);
        if (! strcmp (ref.nome, ctx->EXP) || resPlural (ctx, i, ref.nome))
        {
            valorTk = *(ref.valor);
            if (isdigit (valorTk))
            {
                ctx->token = valorTk;
                while (*ctx->EXP && (isalpha (*ctx->EXP) || *ctx->EXP == ' ')) ctx->EXP++;
                *ctx->EXP = trade;
                ctx->tipoToken = NUM;
                ctx->flagNUM = true;
                listaInsere (ctx, i, ref.nome, ref.valor);
                i = -1;
                if (verificaProxToken (ctx)) return;
            }
            else if (strchr (STR_DEL, valorTk))
            {
                ctx->tipoToken = CONJUCAO;
                while (*ctx->EXP && (isalpha (*ctx->EXP) || *ctx->EXP == ' ' || *ctx->EXP == '-')) ctx->EXP++;
                ctx->token = valorTk;
                *ctx->EXP = trade;
                if (i != CONJUCAO)
                {
                    ctx->tipoToken = DELIMITADOR;
                    ctx->flagNUM = false;
                    return;
                }
                else
                {
                    listaInsere (ctx, i, ref.nome, ref.valor);
                    i = -1;
                }
            }
        }
        else ajustaEXP (ctx);
        i++;
    }
    erroSS (ctx, 0);
}

void ajustaEXP (Context *ctx)
{
    while (*ctx->EXP && *ctx->EXP == ' ') ctx->EXP++;
    int k = strcspn (ctx->EXP, (const char*) " ");
    ctx->EXP[k] = '\0';
}

bool verificaProxToken (Context *ctx)
{
    while (*ctx->EXP && *ctx->EXP == ' ') ctx->EXP++; /* Posiciona a analise no inicio do proximo token */
    char* needle = strchr (ctx->EXP, ' ');
    if (! needle)
    {
        if (*ctx->EXP) return 0;
        return 1;
    }
    int k = needle - ctx->EXP;
    ctx->EXP[k] = '\0';
    char DEL[MAXWLEN] = {'\0'};
    size_t j = ctx->dict->delim_start + 1; /* PULA O 'e' (PRIMEIRO DELIMITADOR) */
    while (j < ctx->dict->n)
    {
        strcpy (DEL, ctx->dict->items[j++].nome);
        char* needle2 = strchr (DEL, '-'); /* TRATA O HIFEN NO DELIMITADOR COMPOSTO */
        if (needle2)
            *needle2 = '\0';
        if (!strcmp (DEL, ctx->EXP))
        {
            ctx->EXP[k] = ' ';
            return 1;
        }
    }
    ctx->EXP[k] = ' ';
    return 0;
}

bool resPlural (Context *ctx, int i, char *currentToken)
{
    (void) i;
    if (! strchr ("mbtqdscount", *currentToken)) return 0;
    char* del = strchr (currentToken, ',');
    bool fl = false;
    if (!del) return 0;
    int k = del - currentToken;
    currentToken[k] = '\0';

    if (! strcmp (currentToken, ctx->EXP))
        fl = true;
    else
    {
        ++del;
        if (! strcmp (del, ctx->EXP))
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

void ajustaDelim (Context *ctx, int* k, char* temp)
{
    if (*ctx->EXP != 'a' && *ctx->EXP != 'f' && *ctx->EXP != 'd' && *ctx->EXP != 'e') return;
    else if (! strcmp (ctx->EXP, (const char*) "abre") || ! strcmp (ctx->EXP, (const char*) "fecha") || \
             ! strcmp (ctx->EXP, (const char*) "dividido") || ! strcmp (ctx->EXP, (const char*) "fatorial") || \
             ! strcmp (ctx->EXP, (const char*) "elevado") \
            )
    {
        int i = 0;
        ctx->EXP[*k] = '-';
        while (isalpha(ctx->EXP[i]) || ctx->EXP[i] == '-') i++;
        *temp = ctx->EXP[i];
        ctx->EXP[i] = '\0';
        if (strcmp (&ctx->EXP[*k+1], (const char*)"parentese") && strcmp (&ctx->EXP[*k+1], (const char*) "de") && \
            strcmp (&ctx->EXP[*k+1], (const char*) "por") && strcmp(&ctx->EXP[*k+1], (const char*) "a")
            )
        {
            ctx->EXP[*k] = ' ';
            erroSS (ctx, 0);
            return;
        }
    }

}

void strToLower (Context *ctx)
{
    int i = 0;
    while(ctx->EXP[i]){
        ctx->EXP[i] = tolower (ctx->EXP[i]);
        i++;
    }
}

#include "extenso/parser.h"
#include "extenso/config.h"
#include "extenso/lexer.h"
#include "extenso/semantics.h"
#include "extenso/conversion.h"
#include "extenso/num_list.h"
#include "extenso/errors.h"
#include "extenso/bignum.h"
#include "extenso/util.h"

char *expParsingStart (Context *ctx)
{
    ctx->flagNUM = false; /* estado limpo (importante ao continuar após um erro) */
    strToLower (ctx);
    char *resposta = (char*) alloc (4*_1KB, sizeof (char));
    char *fResposta = resposta;
    ctx->_TEXP = ctx->EXP;
    pegaToken (ctx);
    if (!ctx->token) erroSS (ctx, 3);
    expResTermo (ctx, resposta);
    if (ctx->token) erroSS (ctx, 0);
    toName (ctx, &resposta);
    ctx->_TEXP = fResposta;
    return resposta;
}

void expResTermo (Context *ctx, char* resposta)
{
    register char op;
    register char* segTermo;
    expResFator (ctx, resposta);
    while ((op = ctx->token) == '+' || op == '-')
    {
        pegaToken (ctx);
        segTermo = (char*) alloc (_1KB, sizeof (char));
        expResFator (ctx, segTermo);
        switch (op)
        {
            case '-':
            memswap(resposta, segTermo, subtrair);
            break;
            case '+':
            memswap(resposta, segTermo, somar);
            break;
        }
        free (segTermo);
    }
}

void expResFator (Context *ctx, char* resposta)
{
    register char op;
    register char* segFator;
    expResFatorial (ctx, resposta);
    while ((op=ctx->token) == '*' || op == '/' || op == '%' || op == '^')
    {
        pegaToken (ctx);
        segFator = (char*) alloc (_1KB, sizeof (char));
        expResFatorial (ctx, segFator);
        switch (op)
        {
            case '*':
            memswap(resposta, segFator, multiplicar);
            break;
            case '/':
            memswapDiv(resposta, segFator, false, unsigneDiv);
            break;
            case '%':
            memswapDiv(resposta, segFator, true, unsigneDiv);
            break;
            case '^':
            memswap(resposta, segFator, unExpo);
            break;
        }
        if (*resposta == 'E') erroSS (ctx, 13);
        free (segFator);
    }
}

void expResFatorial (Context *ctx, char* resposta)
{
    register char* proxFator;
    if (ctx->token == '!')
    {
        pegaToken (ctx);
        proxFator = (char*) alloc (_1KB, sizeof (char));
        expResParenteses (ctx, proxFator);
        char* temp = fatorial (proxFator);
        if (! temp) erroSS (ctx, 8);
        strcpy (resposta, temp);
        free (proxFator);
        free (temp);
        if (!*resposta) erroSS (ctx, 7);
        return;
    }
    expResParenteses (ctx, resposta);
}

void expResParenteses (Context *ctx, char* resposta)
{
    if (ctx->token == '(')
    {
        pegaToken (ctx);
        expResTermo (ctx, resposta);
        if (ctx->token != ')')   erroSS (ctx, 1);
        pegaToken (ctx);
    }
    else atomo (ctx, resposta);
}

void atomo (Context *ctx, char* resposta)
{
    if (ctx->flagNUM)
    {
        if (analiSemantica (ctx))
        {
            char* toNumAnswer = toNum(ctx);
            strcpy (resposta, toNumAnswer);
            free(toNumAnswer);
            listaLibera (ctx);
            ctx->flagNUM = false;
            pegaToken (ctx);
            return;
        }
        erroSS (ctx, 3);
    }
    erroSS (ctx, 0);
}

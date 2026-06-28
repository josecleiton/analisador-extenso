#include "extenso/parser.h"
#include "extenso/bignum.h"
#include "extenso/config.h"
#include "extenso/conversion.h"
#include "extenso/errors.h"
#include "extenso/lexer.h"
#include "extenso/num_list.h"
#include "extenso/semantics.h"
#include "extenso/util.h"

char *
evalExpr (Context *ctx)
{
    ctx->isNumber = false; /* estado limpo (importante ao continuar após um erro) */
    lowercaseExpr (ctx);
    char *answer = (char *)alloc (4 * _1KB, sizeof (char));
    char *fResposta = answer;
    ctx->exprStart = ctx->cursor;
    nextToken (ctx);
    if (!ctx->token)
        reportError (ctx, 3);
    parseTerm (ctx, answer);
    if (ctx->token)
        reportError (ctx, 0);
    toWords (ctx, &answer);
    ctx->exprStart = fResposta;
    return answer;
}

void
parseTerm (Context *ctx, char *answer)
{
    register char op;
    register char *segTermo;
    parseFactor (ctx, answer);
    while ((op = ctx->token) == '+' || op == '-')
        {
            nextToken (ctx);
            segTermo = (char *)alloc (_1KB, sizeof (char));
            parseFactor (ctx, segTermo);
            switch (op)
                {
                case '-':
                    applyInto (answer, segTermo, bigSub);
                    break;
                case '+':
                    applyInto (answer, segTermo, bigAdd);
                    break;
                }
            free (segTermo);
        }
}

void
parseFactor (Context *ctx, char *answer)
{
    register char op;
    register char *segFator;
    parseFactorial (ctx, answer);
    while ((op = ctx->token) == '*' || op == '/' || op == '%' || op == '^')
        {
            nextToken (ctx);
            segFator = (char *)alloc (_1KB, sizeof (char));
            parseFactorial (ctx, segFator);
            switch (op)
                {
                case '*':
                    applyInto (answer, segFator, bigMul);
                    break;
                case '/':
                    applyDivInto (answer, segFator, false, bigDivMod);
                    break;
                case '%':
                    applyDivInto (answer, segFator, true, bigDivMod);
                    break;
                case '^':
                    applyInto (answer, segFator, bigPow);
                    break;
                }
            if (*answer == 'E')
                reportError (ctx, 13);
            free (segFator);
        }
}

void
parseFactorial (Context *ctx, char *answer)
{
    register char *proxFator;
    if (ctx->token == '!')
        {
            nextToken (ctx);
            proxFator = (char *)alloc (_1KB, sizeof (char));
            parseParen (ctx, proxFator);
            char *temp = bigFactorial (proxFator);
            if (!temp)
                reportError (ctx, 8);
            strcpy (answer, temp);
            free (proxFator);
            free (temp);
            if (!*answer)
                reportError (ctx, 7);
            return;
        }
    parseParen (ctx, answer);
}

void
parseParen (Context *ctx, char *answer)
{
    if (ctx->token == '(')
        {
            nextToken (ctx);
            parseTerm (ctx, answer);
            if (ctx->token != ')')
                reportError (ctx, 1);
            nextToken (ctx);
        }
    else
        parseAtom (ctx, answer);
}

void
parseAtom (Context *ctx, char *answer)
{
    if (ctx->isNumber)
        {
            if (checkSemantics (ctx))
                {
                    char *toNumAnswer = toDigits (ctx);
                    strcpy (answer, toNumAnswer);
                    free (toNumAnswer);
                    listFree (ctx);
                    ctx->isNumber = false;
                    nextToken (ctx);
                    return;
                }
            reportError (ctx, 3);
        }
    reportError (ctx, 0);
}

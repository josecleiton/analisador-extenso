#include "extenso/parser.h"
#include "extenso/config.h"
#include "extenso/state.h"
#include "extenso/lexer.h"
#include "extenso/semantics.h"
#include "extenso/conversion.h"
#include "extenso/num_list.h"
#include "extenso/errors.h"
#include "extenso/bignum.h"
#include "extenso/util.h"

char *expParsingStart (void)
{
    strToLower ();
    char *resposta = (char*) alloc (4*_1KB, sizeof (char));
    char *fResposta = resposta;
    dicionario = openFile (ARQ_DICT, "rb");
    ind = criaIndices (dicionario, TAM_DICT).index;
    _TEXP = EXP;
    pegaToken ();
    if (!token) erroSS(3);
    expResTermo (resposta);
    if (token) erroSS (0);
    toName (&resposta);
    _TEXP = fResposta;
    free (ind);
    fclose (dicionario);
    return resposta;
}

void expResTermo (char* resposta)
{
    register char op;
    register char* segTermo;
    expResFator (resposta);
    while ((op = token) == '+' || op == '-')
    {
        pegaToken ();
        segTermo = (char*) alloc (_1KB, sizeof (char));
        expResFator (segTermo);
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

void expResFator (char* resposta)
{
    register char op;
    register char* segFator;
    expResFatorial (resposta);
    while ((op=token) == '*' || op == '/' || op == '%' || op == '^')
    {
        pegaToken ();
        segFator = (char*) alloc (_1KB, sizeof (char));
        expResFatorial (segFator);
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
        if (*resposta == 'E') erroSS (13);
        free (segFator);
    }
}

void expResFatorial (char* resposta)
{
    register char* proxFator;
    if (token == '!')
    {
        pegaToken ();
        proxFator = (char*) alloc (_1KB, sizeof (char));
        expResParenteses (proxFator);
        char* temp = fatorial (proxFator);
        if (! temp) erroSS (8);
        strcpy (resposta, temp);
        free (proxFator);
        free (temp);
        if (!*resposta) erroSS (7);
        return;
    }
    expResParenteses (resposta);
}

void expResParenteses (char* resposta)
{
    if (token == '(')
    {
        pegaToken ();
        expResTermo (resposta);
        if (token != ')')   erroSS (1);
        pegaToken ();
    }
    else atomo (resposta);
}

void atomo (char* resposta)
{
    if (flagNUM)
    {
        if (analiSemantica ())
        {
            char* toNumAnswer = toNum();
            strcpy (resposta, toNumAnswer);
            free(toNumAnswer);
            listaLibera ();
            flagNUM = false;
            pegaToken ();
            return;
        }
        erroSS (3);
    }
    erroSS (0);
}

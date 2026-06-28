#include "extenso/lexer.h"
#include "extenso/state.h"
#include "extenso/tokens.h"
#include "extenso/num_list.h"
#include "extenso/errors.h"

void pegaToken (void)
{
    Ordem ref;
    int i = 0, k = 0;
    char trade = '\0';
    char valorTk = '\0';
    token = '\0';
    tipoToken = 0;
    if (!*EXP) return;
    while (isspace (*EXP)) ++EXP;
    while (EXP[k] && isalpha (EXP[k])) k++;
    trade = EXP[k];
    EXP[k] = '\0';
    ajustaDelim (&k, &trade);
    while (i < (int) dict->n)
    {
        strcpy (ref.nome, dict->items[i].nome);
        strcpy (ref.valor, dict->items[i].valor);
        if (! strcmp (ref.nome, EXP) || resPlural (i, ref.nome))
        {
            valorTk = *(ref.valor);
            if (isdigit (valorTk))
            {
                token = valorTk;
                while (*EXP && (isalpha (*EXP) || *EXP == ' ')) EXP++;
                *EXP = trade;
                tipoToken = NUM;
                flagNUM = true;
                listaInsere (i, ref.nome, ref.valor);
                i = -1;
                if (verificaProxToken ()) return;
            }
            else if (strchr (STR_DEL, valorTk))
            {
                tipoToken = CONJUCAO;
                while (*EXP && (isalpha (*EXP) || *EXP == ' ' || *EXP == '-')) EXP++;
                token = valorTk;
                *EXP = trade;
                if (i != CONJUCAO)
                {
                    tipoToken = DELIMITADOR;
                    flagNUM = false;
                    return;
                }
                else
                {
                    listaInsere (i, ref.nome, ref.valor);
                    i = -1;
                }
            }
        }
        else ajustaEXP ();
        i++;
    }
    erroSS (0);
}

void ajustaEXP (void)
{
    while (*EXP && *EXP == ' ') EXP++;
    int k = strcspn (EXP, (const char*) " ");
    EXP[k] = '\0';
}

bool verificaProxToken (void)
{
    while (*EXP && *EXP == ' ') EXP++; /* Posiciona a analise no inicio do proximo token */
    char* needle = strchr (EXP, ' ');
    if (! needle)
    {
        if (*EXP) return 0;
        return 1;
    }
    int k = needle - EXP;
    EXP[k] = '\0';
    char DEL[MAXWLEN] = {'\0'};
    size_t j = dict->delim_start + 1; /* PULA O 'e' (PRIMEIRO DELIMITADOR) */
    while (j < dict->n)
    {
        strcpy (DEL, dict->items[j++].nome);
        char* needle2 = strchr (DEL, '-'); /* TRATA O HIFEN NO DELIMITADOR COMPOSTO */
        if (needle2)
            *needle2 = '\0';
        if (!strcmp (DEL, EXP))
        {
            EXP[k] = ' ';
            return 1;
        }
    }
    EXP[k] = ' ';
    return 0;
}

bool resPlural (int i, char *currentToken)
{
    (void) i;
    if (! strchr ("mbtqdscount", *currentToken)) return 0;
    char* del = strchr (currentToken, ',');
    bool fl = false;
    if (!del) return 0;
    int k = del - currentToken;
    currentToken[k] = '\0';

    if (! strcmp (currentToken, EXP))
        fl = true;
    else
    {
        ++del;
        if (! strcmp (del, EXP))
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

void ajustaDelim (int* k, char* temp)
{
    if (*EXP != 'a' && *EXP != 'f' && *EXP != 'd' && *EXP != 'e') return;
    else if (! strcmp (EXP, (const char*) "abre") || ! strcmp (EXP, (const char*) "fecha") || \
             ! strcmp (EXP, (const char*) "dividido") || ! strcmp (EXP, (const char*) "fatorial") || \
             ! strcmp (EXP, (const char*) "elevado") \
            )
    {
        int i = 0;
        EXP[*k] = '-';
        while (isalpha(EXP[i]) || EXP[i] == '-') i++;
        *temp = EXP[i];
        EXP[i] = '\0';
        if (strcmp (&EXP[*k+1], (const char*)"parentese") && strcmp (&EXP[*k+1], (const char*) "de") && \
            strcmp (&EXP[*k+1], (const char*) "por") && strcmp(&EXP[*k+1], (const char*) "a")
            )
        {
            EXP[*k] = ' ';
            erroSS(0);
            return;
        }
    }

}

void strToLower (void)
{
    int i = 0;
    while(EXP[i]){
        EXP[i] = tolower (EXP[i]);
        i++;
    }
}

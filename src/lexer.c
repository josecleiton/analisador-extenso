#include "extenso/lexer.h"
#include "extenso/state.h"
#include "extenso/tokens.h"
#include "extenso/num_list.h"
#include "extenso/errors.h"

void pegaToken (void)
{
    rewind (dicionario);
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
    while (!feof (dicionario) && i < TAM_DICT)
    {
        fscanf (dicionario, "%[^=]=%[^\n]%*c", ref.nome, ref.valor);
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
                rewind (dicionario);
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
                    rewind (dicionario);
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
    int i = 1; /* COMEÇA EM UM PORQUE O PRIMEIRO DELIMITADOR É O 'e' */
    char DEL[20] = {'\0'};
    while (i < (TAM_DICT - INDEL+1) && !feof (dicionario))
    {
        fseek (dicionario, ind[INDEL+(i++)], SEEK_SET);
        fscanf (dicionario, "%[^=]", DEL);
        char* needle2 = strchr (DEL, '-'); /* TRATA O HIFEN NO DELIMITADOR COMPOSTO */
        if (needle2)
            *needle2 = '\0';
        if (!strcmp (DEL, EXP))
        {
            EXP[k] = ' ';
            rewind (dicionario);
            return 1;
        }
    }
    EXP[k] = ' ';
    rewind (dicionario);
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

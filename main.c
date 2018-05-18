/*
    Universidade Estadual do Estado da Bahia - UNEB
    Por: José Cleiton Borges Correia
    Prof: Jorge Sampaio Farias
    Disciplina: Linguagem de Programação 1 - 2018.1
    #################################################
    | AVALIADOR DE EXPRESSÕES NUMERICAS POR EXTENSO |
    | Entrada: expressão numérica por extenso       |
    | Saída ideal: expressão resolvida por extenso  |
    | Saída realística: expressão númerica          |
    #################################################
*/

#ifndef INCLUSOS
    #define INCLUSOS
    #include "lib/preproc.h"
    #define ERRO exit (3141592)
#endif
#include "lib/operacoes.c"
#include "lib/cardnumeral.c"
#define ARQ_ORDENS "lib/ordens.txt"
#define ARQ_ENTRADA "lib/expressoes.txt"
#define ARQ_SAIDA "lib/resultados.txt"
#define ARQ_ERROS "lib/erros.txt"

#define TAM 26
#define NUM_ERROS 5
/*
struct ordem
{
    char* nome;
    char* valor;
};*/

enum tokens
{
    NUM=1,
    CONJUCAO,
    DELIMITADOR, /* 3 */
    MILHAO = 38,
    BILHAO,
    TRILHAO,
    QUATRILHAO,
    QUINTILHAO,
    SEXTILHAO, /*43*/
};

/* 
*
*       VARIAVEIS GLOBAIS 
*
*/
Ordem* ref;
char *EXP, *_TEXP, *NUMERO;
char token[30], flagNUM, tk_tmp[60];
char tipoToken;
char strErro[300];

Ordem* cria_dic (void); /* DICIONÁRIO UTILIZADO (CHECAR lib/ordens.txt) */
void expParsingStart (char resultado[]); /* GATILHO DE PARTIDA */
void expResTermo (char resultado[]); /* ROTINA QUE SOMA OU SUBTRAI TERMOS */
void expResFator (char resultado[]); /* ROTINA QUE DIVIDE OU MULTIPLICA FATORES */
void expResFatorial (char resultado[]); /* ROTINA QUE RESOLVE O FATORIAL DE UM FATOR */
void expResParenteses (char resultado[]); /* ROTINA QUE RESOLVE UMA EXPRESSÃO DENTRO DE PARENTESES */
void expAvalSinal (char resultado[]); /* AVALIA + OU - UNÁRIO */
void atomo (char resultado[]); /* DEVOLVE O VALOR NUMERICO DAS EXPRESSÕES POR EXTENSO*/
char* get_token (void); /* PEGA O PROX TOKEN */
void getNumber (char resultado[]); /* PEGA TODO UM NUMERO POR EXTENSO */
void ajustaDelim (int* k, char* temp); /* AJUSTA DELIMITADORES COMPOSTOS COM HÍFEN ENTRE AS PALAVRAS */
void erroSS (int tipoErro); /* TODOS OS POSSÍVEIS ERROS (CHECAR lib/erros.txt) */
void criaIndices (FILE* in, Int2B** out, int size);
int compara (char* s1, char* s2); /* VERSÃO ADAPTADA DO strcmp */

int main (void)
{
    MALLOC (EXP, TAM*10);
    char* resultado;
    scanf("%[^\n]", EXP);
    MALLOC (resultado, strlen(EXP)*2);
    expParsingStart (resultado);
    puts (resultado);
    return 0;
}

Ordem* cria_dic (void)
{
    FILE* nomes;
    Ordem* ref;
    OPENFILE (nomes, ARQ_ORDENS, "r");
    MALLOC (ref, sizeof(Ordem)*TAM*2);
    char i = 0;
    while (! feof(nomes))
    {
        MALLOC (ref[i].nome, TAM);
        MALLOC (ref[i].valor, TAM);
        if (fscanf (nomes, "%[^=]=%[^\n]%*c", ref[i].nome, ref[i].valor) >= sizeof(Ordem)) break;
        i++;
    }
    fclose (nomes);
    return ref;
}

void expParsingStart (char resultado[])
{
    ref = cria_dic ();
    _TEXP = EXP;
    *token = '\0';
    NUMERO = token;
    /*get_token();*/
    getNumber (resultado);
    if (!*token)
    {
        erroSS(3);
        return;
    }
    if (*token) erroSS (0);
}

void expResTermo (char resultado[])
{
    register char op = *NUMERO;
    char segTermo[300];
    expResFator (resultado);
    while (op == '+' || op == '-')
    {
        getNumber (segTermo);
        expResFator (segTermo);
        switch (op)
        {
            case '-':
            subtrair (resultado, segTermo);
            break;
            case '+':
            soma (resultado, segTermo);
            break;
        }
        break; 
    }
}

void expResFator (char resultado[])
{
    register char op = *NUMERO;
    char segFator[300];
    expResFatorial (resultado);
    while (op == '*' || op == '/')
    {
        getNumber (segFator);
        expResFatorial (segFator);
        switch (op)
        {
            case '*':
            multiplica (resultado, segFator);
            break;
            case '/':
            divide (resultado, segFator);
            break;
        }
        break;
    }
}

void expResFatorial (char resultado[])
{
    char proxFator[300];
    expResParenteses (resultado);
    if (*token == '!')
    {
        getNumber (proxFator);
        if (*proxFator == '-')
        {
            erroSS (4);
            return;
        }
    }
}

void expAvalSinal (char resultado[])
{
    register char op;
    char proxToken[300];
    char* tempPT = proxToken+1;
    op = 0;
    if ((tipoToken == DELIMITADOR) && *token=='+' || *token=='-')
    {
        op = *token;
        getNumber (tempPT);
    }
    expResParenteses (tempPT);
    if (op=='-')
        *--tempPT = '-';
    /* NADA DE INVERSÃO */
}

void expResParenteses (char resultado[])
{
    char proxToken[300];
    if (*token == '(')
    {
        getNumber (resultado);
        expResTermo (resultado);
        if (*token != ')')
            erroSS (1);
        getNumber (proxToken);
    }
    else atomo (resultado);
}

void atomo (char resultado[])
{
    char proxToken [300];
    if (flagNUM == 1)
    {
        resultado = analiSemantica (tk_tmp, ref);
        *tk_tmp = '\0';
        return;
    }
    erroSS (0);
}

void erroSS (int tipoErro)
{
    FILE* erroS;
    OPENFILE (erroS, ARQ_ERROS, "rb");
    int temp, i = 0, tamErro;
    if (! tipoErro)
    {
        fscanf (erroS, "%[^\n]%c", strErro);
        strcat (strErro, "\n\t");
        strcat (strErro, _TEXP);
        strcat (strErro, "\n\t");
        temp = EXP - _TEXP;
        tamErro = strlen (strErro);
        while (i < temp)
        {
            strErro[tamErro+i] = ' ';
            i++;
        } 
        strErro[tamErro+i] = '^';
        strErro[tamErro+i+1] = '\n';
    }
    else
    {
        Int2B *idc = NULL;
        criaIndices (erroS, &idc, NUM_ERROS);
        if (tipoErro%2)
        {
            fseek (erroS, idc[tipoErro-1], SEEK_SET);
            fscanf (erroS, "%[^\n]%c", strErro);
            strcat (strErro, "\n\t");
            strcat (strErro, _TEXP);
            strcat (strErro, "\n\t");
            temp = EXP - _TEXP;
            tamErro = strlen (strErro);
            while (i < temp)
            {
                strErro[tamErro+i] = ' ';
                i++;
            } 
            strErro[tamErro+i] = '^';
            strErro[tamErro+i+1] = '\n';
        }
    }
    puts (strErro);
    fclose (erroS);
    ERRO;
}

void criaIndices (FILE* in, Int2B** out, int size)
{
    Int2B *ind, i=0;
    MALLOC(ind, sizeof(Int2B)*size);
    char ch = getc (in);
    while (ch != EOF)
    {
        if (ch == '\n')
        {
            ind[i] = (Int2B) ftell (in);
            i++;
        }
        ch = getc (in);
    }
    *out = ind;
}

void getNumber (char resultado[])
{
    register char *temp;
    char* ptr = tk_tmp;
    int count = 0;
    if (!tipoToken || tipoToken == DELIMITADOR) /*QUER DIZER UM NOVO NUMERO */
    {
        *token = '\0';
        NUMERO = temp = token;
        tipoToken = 0;
    }
    while (tipoToken != DELIMITADOR)
    {
        if (count)
        { 
            strcat (tk_tmp, token);
            strcat (tk_tmp, (char*) "-");
            temp = get_token ();
        }
        else get_token ();
        if (!tipoToken) break;
        /*if (tipoToken != DELIMITADOR) strcat (token, temp);*/
        count++;
    }
    if (!*tk_tmp)
    {
        erroSS (5);
        return;
    }
    expResTermo (resultado);
    flagNUM = 0;
}

char* get_token (void)
{
    register char *temp;
    int i;
    tipoToken = 0;
    if (!tipoToken || tipoToken == DELIMITADOR) /*QUER DIZER UM NOVO NUMERO */
    {
        *token = '\0';
        NUMERO = temp = token;
        tipoToken = 0;
    }
    if (!*EXP) return NULL; /* SE FOR A EXPRESSÃO FOR VAZIA */
    while (isspace (*EXP)) /* IGNORA OS ESPAÇOS */
        ++EXP;
    if (!*EXP) return NULL;
    int k = 0;
    char chEXP;
    while (EXP[k] && isalpha (EXP[k])) k++;
    chEXP = EXP[k];
    EXP[k] = '\0';
    ajustaDelim (&k, &chEXP);
    for (i=0; i<TAM*2; i++)
    {
        if (strstr (ref[i].nome, EXP)) /*SE ELE FOR UM NUMERO*/
        {
            int j = 0;
            if (isdigit (ref[i].valor[0]))
            {
                strcat (temp, EXP);
                NUMERO = temp + strlen (EXP); /* CURSOR FICA NO FIM DE TOKEN */
                while (*EXP && (isalpha (*EXP))) EXP++;
                *EXP = chEXP;
                /* PROCURAR FUNÇÃO QUE POSICIONE O PONTEIRO TEMP APÓS A ULTIMA LETRA DE REF[I].NOME */
                tipoToken = NUM;
                flagNUM = 1; /*AQ*/
                if (!*NUMERO) return temp;
                return NUMERO;
            }
            else if (strchr("()+-*/!e", ref[i].valor[0]))
            {
                tipoToken = CONJUCAO;
                while (*EXP && (isalpha (*EXP) || isspace (*EXP) || *EXP == '-'))
                {
                    EXP++;
                }
                NUMERO = temp;
                *temp++ = ref[i].valor[0];
                *temp = '\0';
                *EXP = chEXP; /* RECOLOCANDO O NULO OU O ESPAÇO NO DEVIDO LUGAR */
                if (i!=CONJUCAO) tipoToken = DELIMITADOR;
                return NUMERO;
            }
        }
    }
}

void ajustaDelim (int* k, char* temp) /* COLOCA UM HIFEN ENTRE OS DELIMITADORES COMPOSTOS */
{
    /*
    *   DELIMITADORES COMPOSTOS:
    *   dividido-por
    *   fatorial-de
    *   abre-parentese
    *   fecha-parentese
    */
    if (*EXP != 'a' && *EXP != 'f' && *EXP != 'd') return;
    else if (! strcmp (EXP, (char*) "abre") || ! strcmp (EXP, (char*) "fecha") || ! strcmp (EXP, (char*) "dividido") || ! strcmp (EXP, (char*) "fatorial"))
    {
        int i = 0;
        EXP[*k] = '-';
        while (isalpha(EXP[i]) || EXP[i] == '-') i++;
        *temp = EXP[i];
        EXP[i] = '\0';
        if (strcmp (&EXP[*k+1], (char*) "parentese"))
        {
            EXP[*k] = ' '; 
            erroSS(0);
            return;
        }
    }

}

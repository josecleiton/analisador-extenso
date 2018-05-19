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
    DELIMITADOR, /* 3 */
    MIL = 37,
    MILHAO,
    BILHAO,
    TRILHAO,
    QUATRILHAO,
    QUINTILHAO,
    SEXTILHAO, /*43*/
    CONJUCAO
};

/* 
*
*       VARIAVEIS GLOBAIS 
*
*/
Ordem* ref;
char *EXP, *_TEXP, *NUMERO, expNum[300];
char token[30], flagNUM, tk_tmp[60];
char tipoToken;
char strErro[300];
FILE* dicionario;
FilaNum* queue;

//Ordem* cria_dic (void); /* DICIONÁRIO UTILIZADO (CHECAR lib/ordens.txt) */
void expParsingStart (char* resposta); /* GATILHO DE PARTIDA */
void expResTermo (char* resposta); /* ROTINA QUE SOMA OU SUBTRAI TERMOS */
void expResFator (char* resposta); /* ROTINA QUE DIVIDE OU MULTIPLICA FATORES */
void expResFatorial (char* resposta); /* ROTINA QUE RESOLVE O FATORIAL DE UM FATOR */
void expResParenteses (char* resposta); /* ROTINA QUE RESOLVE UMA EXPRESSÃO DENTRO DE PARENTESES */
void expAvalSinal (char* resposta); /* AVALIA + OU - UNÁRIO */
void atomo (char* resposta); /* DEVOLVE O VALOR NUMERICO DAS EXPRESSÕES POR EXTENSO*/
char* get_token (void); /* PEGA O PROX TOKEN */
int resPlural (int i);
void getNumber (char* resposta); /* PEGA TODO UM NUMERO POR EXTENSO */
void ajustaDelim (int* k, char* temp); /* AJUSTA DELIMITADORES COMPOSTOS COM HÍFEN ENTRE AS PALAVRAS */
void erroSS (int tipoErro); /* TODOS OS POSSÍVEIS ERROS (CHECAR lib/erros.txt) */
void criaIndices (FILE* in, Int2B** out, int size);
int compara (char* s1, char* s2); /* VERSÃO ADAPTADA DO strcmp */

void filaInsere (int i, char* nome, char* valor);
void filaLibera (void);

int analiSemantica (void);
char* toNumber (void);
void initString (char** s);

int main (void)
{
    EXP = expNum;
    char* resultado;
    scanf("%[^\n]", EXP);
    MALLOC (resultado, strlen(EXP)*2);
    expParsingStart (resultado);
    puts (resultado);
    return 0;
}
/*
Ordem* cria_dic (void)
{
    FILE* nomes;
    Ordem* ref;
    OPENFILE (nomes, ARQ_ORDENS, "r");
    MALLOC (ref, sizeof(Ordem)*TAM*2);
    char i = 0;
    while (! feof(nomes))
    {
        MALLOC (ref[i].nome, 22);
        MALLOC (ref[i].valor, 22);
        if (fscanf (nomes, "%[^=]=%[^\n]%*c", ref[i].nome, ref[i].valor) >= sizeof(Ordem)) break;
        i++;
    }
    fclose (nomes);
    return ref;
}
*/
void expParsingStart (char* resposta)
{
    //ref = cria_dic ();
    OPENFILE (dicionario, ARQ_ORDENS, "r");
    MALLOC (ref, sizeof(Ordem));
    _TEXP = EXP;
    *token = '\0';
    NUMERO = token;
    /*get_token();*/
    getNumber (resposta);
    if (!*token && !*resposta)
    {
        erroSS(3);
        return;
    }
    if (*token) erroSS (0);
    fclose (dicionario);
}

void expResTermo (char* resposta)
{
    register char op = *NUMERO;
    //char segTermo[300];
    char* segTermo;
    expResFator (resposta);
    while (op == '+' || op == '-')
    {
        getNumber (segTermo);
        expResFator (segTermo);
        switch (op)
        {
            case '-':
            subtrair (resposta, segTermo);
            break;
            case '+':
            soma (resposta, segTermo);
            break;
        }
        break; 
    }
}

void expResFator (char* resposta)
{
    register char op = *NUMERO;
    //char segFator[300];
    char* segFator;
    expResFatorial (resposta);
    while (op == '*' || op == '/')
    {
        getNumber (segFator);
        expResFatorial (segFator);
        switch (op)
        {
            case '*':
            multiplica (resposta, segFator);
            break;
            case '/':
            divide (resposta, segFator);
            break;
        }
        break;
    }
}

void expResFatorial (char* resposta)
{
    //char proxFator[300];
    char* proxFator;
    expAvalSinal (resposta);
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

void expAvalSinal (char* resposta)
{
    register char op = 0;
    //char proxToken[300];
    char* proxToken;
    char* tempPT;
    if ((tipoToken == DELIMITADOR) && *token=='+' || *token=='-')
    {
        tempPT = proxToken + 1;
        op = *token;
        getNumber (tempPT);
    }
    expResParenteses (resposta);
    if (op=='-')
    {
        *--tempPT = '-';
        strcpy (resposta, tempPT);
    }
}

void expResParenteses (char* resposta)
{
    char* proxToken;
    if (*token == '(')
    {
        getNumber (resposta);
        expResTermo (resposta);
        if (*token != ')')
            erroSS (1);
        getNumber (proxToken);
    }
    else atomo (resposta);
}

void atomo (char* resposta)
{
    char proxToken [300];
    if (flagNUM == 1)
    {
        if (analiSemantica ())
        {
            char* temp = toNumber();
            strcpy (resposta, temp);
            filaLibera ();
            *tk_tmp = '\0';
            return;
        }
        erroSS (3);
    }
    erroSS (0);
}

int analiSemantica (void)
{
    return 1;
}

char* toNumber (void)
{
    char *resultado = (char*) malloc (2), *guardaClasse = (char*) malloc (2);
    if (!resultado || !guardaClasse) ERRO;
    resultado[0] = guardaClasse[0] = '0';
    resultado[1] = guardaClasse[1] = '\0';
    while (queue)
    {
        int k = queue -> classe;
        if (k!=CONJUCAO)
        {
            if (k<MIL)
            {
                guardaClasse = soma (queue->info->valor, guardaClasse);
                while (*guardaClasse == '0') guardaClasse++;
            }
            else
            {
                char* temp = multiplica (queue->info->valor, guardaClasse);
                while (*temp == '0') temp++;
                resultado = soma (temp, resultado);
                while (*resultado == '0') resultado++;
                strcpy (guardaClasse, (char*) "0");
            }
        }
        queue = queue -> prox;
    }
    return resultado;
}

void initString (char** s)
{
    if (!*s) *s = (char*) malloc (3);
    strcpy (*s, (char*) "0");
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

void getNumber (char* resposta)
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
    if (!*token && !*tk_tmp)
    {
        erroSS (5);
        return;
    }
    expResTermo (resposta);
    flagNUM = 0;
}

char* get_token (void)
{
    register char *temp;
    int i=0;
    rewind (dicionario);
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
    while (!feof (dicionario) && i<TAM*2)
    {
        MALLOC (ref->nome, 22);
        MALLOC (ref->valor, 22)
        fscanf (dicionario, "%[^=]=%[^\n]%*c", ref->nome, ref->valor);
        if (! strcmp (ref->nome, EXP) || resPlural(i))
        {
            if (isdigit (ref->valor[0]))
            {
                strcat (temp, EXP);
                NUMERO = temp + strlen (EXP);
                while (*EXP && (isalpha (*EXP))) EXP++;
                *EXP = chEXP;
                tipoToken = NUM;
                flagNUM = 1;
                filaInsere (i, ref->nome, ref->valor);
                return temp;
            }
            else if (strchr ("+-/*!e", ref->valor[0]))
            {
                tipoToken = CONJUCAO;
                while (*EXP && (isalpha (*EXP) || isspace (*EXP) || *EXP == '-'))
                {
                    EXP++;
                }
                NUMERO = temp;
                *temp++ = ref->valor[0];
                *temp = '\0';
                *EXP = chEXP; /* RECOLOCANDO O NULO OU O ESPAÇO NO DEVIDO LUGAR*/
                if (i!=CONJUCAO) tipoToken = DELIMITADOR;
                else filaInsere(i, ref->nome, ref->valor);
                return NUMERO;
            }
        }
        free (ref->nome);
        free (ref->valor);
        i++;

    }
}

int resPlural (int i)
{
    if (! strchr ("mbtqs", ref->nome[0])) return 0;
    int j=0;
    char* del = strpbrk (ref->nome, (char*) ",");
    char fl = 0;
    if (!del) return 0;
    int k = del - ref->nome;
    ref->nome[k] = '\0';

    if (! strcmp (ref->nome, EXP))
    {
        fl = 1;
    }
    else
    {
        ++del;
        if (! strcmp (del, EXP)) fl = 1;
    }
    ref->nome[k] = ',';
    if (fl) return 1;
    return 0;
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

void filaInsere (int i, char* nome, char* valor)
{
    FilaNum *no, *aux = queue;
    MALLOC(no, sizeof (FilaNum));
    no -> info = (Ordem*) malloc (sizeof(Ordem));
    if (no->info == NULL) ERRO;
    no->info->nome = nome;
    no->info->valor = valor;
    no -> classe = i;
    no -> prox = NULL;
    if (! queue)
    {
        queue = no;
        return;
    }
    while (aux && aux->prox)
        aux = aux -> prox;
    aux -> prox = no;
}

void filaLibera (void)
{
    FilaNum *aux = queue, *aux2;
    while (aux)
    {
        aux2 = aux;
        aux = aux -> prox;
        free (aux2);
    }
    queue = NULL;
}


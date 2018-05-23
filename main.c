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

/* 
RESOLVER:
abre parentese treze mais cinco vezes abre parentese cinco menos um mais quatro fecha parentese mais/menos quatro fecha parentese
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

typedef struct ordem Ordem;
typedef struct filanum FilaNum;
struct ordem
{
    char* nome;
    char* valor;
};
struct filanum
{
    int classe;
    Ordem *info;
    FilaNum *ant, *prox;
};

enum tokens
{
    ZERO, UM, DOIS, TRES, QUATRO, CINCO, SEIS, SETE, OITO, NOVE, DEZ, ONZE, DOZE, TREZE, CATORZE, QUINZE, DEZESSEIS, DEZESSETE, DEZOITO, DEZENOVE, VINTE, TRINTA, QUARENTA, CINQUENTA,SESSENTA, SETENTA, OITENTA, NOVENTA, CEM, DUZENTOS, TREZENTOS, QUATROCENTOS, QUINHENTOS, SEISCENTOS, SETECENTOS, OITOCENTOS, NOVECENTOS, MIL, MILHAO, BILHAO, TRILHAO, QUATRILHAO, QUINTILHAO, SEXTILHAO, CONJUCAO, NUM, DELIMITADOR
};

/* 
*
*       VARIAVEIS GLOBAIS 
*
*/
Ordem* ref;
char *EXP, *_TEXP, *NUMERO, expNum[300];
char token[30], flagNUM, tk_tmp[60];
char tipoToken, fimEXP;
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
int resPlural (int i, char** s); /* EM ORDENS COMPOSTAS, AVALIA TANTO A FORMA PLURAL QUANTO SINGULAR E ENFILA A FORMA INSERIDA */
void getNumber (char* resposta); /* PEGA TODO UM NUMERO POR EXTENSO */
void ajustaDelim (int* k, char* temp); /* AJUSTA DELIMITADORES COMPOSTOS COM HÍFEN ENTRE AS PALAVRAS */
void erroSS (int tipoErro); /* TODOS OS POSSÍVEIS ERROS (CHECAR lib/erros.txt) */
void criaIndices (FILE* in, Int2B** out, int size);
int compara (char* s1, char* s2); /* VERSÃO ADAPTADA DO strcmp */

void filaInsere (int i, char* nome, char* valor);
void filaLibera (void);
int filaCount (void);

int analiSemantica (void);
char* toNumber (void);
void initString (char** s);

int main (void)
{
    EXP = expNum;
    char* resultado;
    scanf("%[^\n]", EXP);
    MALLOC (resultado, 1024);
    expParsingStart (resultado);
    puts (resultado);
    return 0;
}

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
    if (!*token && !*resposta && !fimEXP)
    {
        erroSS(3);
        return;
    }
    else if (*token && !fimEXP) erroSS (0);
    fclose (dicionario);
}

void expResTermo (char* resposta)
{
    register char op = *token;
    //char segTermo[300];
    char* segTermo;
    expResFator (resposta);
    while (op == '+' || op == '-' || *token == '+' || *token == '-')
    {
        if (op != '+' || op != '-') op = *token;
        MALLOC (segTermo, strlen(EXP));
        getNumber (segTermo);
        //expResFator (segTermo);
        switch (op)
        {
            case '-':
            strcpy (resposta, subtrair (resposta, segTermo));
            break;
            case '+':
            strcpy (resposta, soma (resposta, segTermo));
            break;
        }
        free (segTermo);
        break; 
    }
}

void expResFator (char* resposta)
{
    register char op = *token;
    //char segFator[300];
    char* segFator;
    expResFatorial (resposta);
    while (op == '*' || op == '/' || *token == '*' || *token == '/')
    {
        if (op!='*' || op != '/') op = *token;
        MALLOC (segFator, strlen(EXP));
        getNumber (segFator);
        //expResFatorial (segFator);
        switch (op)
        {
            case '*':
            strcpy (resposta, multiplica (resposta, segFator));
            return;
            case '/':
            strcpy (resposta, divide (resposta, segFator));
            return;
        }
        break;
    }
}

void expResFatorial (char* resposta)
{
    register char op = *token;
    char* proxFator;
    if (op == '!')
    {
        MALLOC (proxFator, 300);
        getNumber (proxFator);
        strcpy (resposta, fatorial (proxFator));
        if (!*resposta)
            erroSS (7);
        free (proxFator);
        if (*proxFator == '-')
        {
            erroSS (4);
            return;
        }
        return;
    }
    expResParenteses (resposta);
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
        //expResTermo (resposta);
        if (*token != ')')
            erroSS (1);
        MALLOC (proxToken, strlen(EXP));
        get_token ();
    }
    else atomo (resposta);
}

void atomo (char* resposta)
{
    /*char proxToken [300];*/
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
    else 
    {
        if (strchr ("*/", *token))
        {
            *resposta = '1'; /*neutro da multiplicação*/
            *(resposta+1) = '\0';
            /* verifique aqui se for o inicio da expressão */
            return;
        }
        else if (strchr ("+-", *token)) return;
    }
    erroSS (0);
}

int analiSemantica (void)
{
    return 1;
}

char* toNumber (void)
{
    char *resultado = NULL, *guardaClasse = NULL;
    initString (&resultado);
    initString (&guardaClasse);
    while (queue)
    {
        int k = queue -> classe;
        if (k!=CONJUCAO)
        {
            if (k<MIL)
            {
                char* temp = guardaClasse;
                guardaClasse = soma (queue->info->valor, guardaClasse);
                while (*guardaClasse == '0') guardaClasse++;
            }
            else
            {
                char* temp = multiplica (queue->info->valor, guardaClasse);
                while (*temp == '0') temp++;
                resultado = soma (temp, resultado);
                if (temp && *temp) free (temp);
                while (*resultado == '0') resultado++;
                initString (&guardaClasse);
            }
        }
        queue = queue -> prox;
    }
    if (*resultado == '0')
    {
        free (resultado);
        resultado = guardaClasse;
    }
    else if (*guardaClasse != '0')
    {
        resultado = soma (resultado, guardaClasse);
        *guardaClasse = '\0';
    }
    return resultado;
}

void initString (char** s)
{
    if (!*s)
    {
        *s = (char*) malloc (2);
        if (!*s) ERRO;
    }
    (*s)[0] = '0';
    (*s)[1] = '\0';
}

void erroSS (int tipoErro)
{
    FILE* erroS;
    OPENFILE (erroS, ARQ_ERROS, "rb");
    int temp, i = 0, tamErro;
    Int2B *idc = NULL;
    criaIndices (erroS, &idc, NUM_ERROS);
    if (tipoErro%2 || ! tipoErro)
    {
        fseek (erroS, idc[tipoErro], SEEK_SET);
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
    puts (strErro);
    free (idc);
    fclose (erroS);
    ERRO;
}

void criaIndices (FILE* in, Int2B** out, int size)
{
    Int2B *ind, i=1;
    MALLOC(ind, sizeof(Int2B)*++size);
    *ind = 0;
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
        temp = token;
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
    if (!*token && !*tk_tmp && !fimEXP)
    {
        erroSS (5);
        return;
    }
    else if (filaCount() == 1 || !fimEXP || tipoToken == DELIMITADOR) expResTermo (resposta);
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
    if (!*EXP)
    {
        if (ref->valor) fimEXP = 1;
        return NULL; /* SE FOR A EXPRESSÃO FOR VAZIA */
    }
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
        MALLOC (ref->nome, TAM);
        MALLOC (ref->valor, TAM)
        fscanf (dicionario, "%[^=]=%[^\n]%*c", ref->nome, ref->valor);
        if (! strcmp (ref->nome, EXP) || resPlural(i, &ref->nome))
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
            else if (strchr ("+-/*!e()", ref->valor[0]))
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
    erroSS (0);
}

int resPlural (int i, char** s)
{
    char *nome = *s;
    if (! strchr ("mbtqs", nome[0])) return 0;
    int j=0;
    char* del = strpbrk (nome, (char*) ",");
    char fl = 0;
    if (!del) return 0;
    int k = del - nome;
    nome[k] = '\0';

    if (! strcmp (nome, EXP))
    {
        fl = 1;
    }
    else
    {
        ++del;
        if (! strcmp (del, EXP))
        {
            fl = 1;
            *s = del;
        }
    }
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
        if (strcmp (&EXP[*k+1], (char*) "parentese") && strcmp (&EXP[*k+1], (char*) "de") && strcmp (&EXP[*k+1], (char*) "por"))
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
        no -> ant = NULL;
        return;
    }
    while (aux && aux->prox)
        aux = aux -> prox;
    aux -> prox = no;
    no -> ant = aux;
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

int filaCount (void)
{
    FilaNum* aux;
    int n;
    for (n = 0, aux = queue; aux; aux = aux -> prox, n++);
    return n;
}
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
PROX PASSO: ERRO SEMANTICO EM "CEM E TRES" OU "DOIS MILHAO" OU "UM MILHOES" ETC
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

#define TAM 28
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
    ZERO, UM, DOIS, TRES, QUATRO, CINCO, SEIS, SETE, OITO, NOVE, DEZ, ONZE, DOZE, TREZE, CATORZE, QUINZE, DEZESSEIS, DEZESSETE, DEZOITO, DEZENOVE, VINTE, TRINTA, QUARENTA, CINQUENTA,SESSENTA, SETENTA, OITENTA, NOVENTA, CEM, DUZENTOS, TREZENTOS, QUATROCENTOS, QUINHENTOS, SEISCENTOS, SETECENTOS, OITOCENTOS, NOVECENTOS, MIL, MILHAO, BILHAO, TRILHAO, QUATRILHAO, QUINTILHAO, SEXTILHAO, SETILHAO, OCTILHAO, NONILHAO, DECILHAO, CONJUCAO, NUM, DELIMITADOR
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

void expParsingStart (char* resposta); /* GATILHO DE PARTIDA */
void expResTermo (char* resposta); /* ROTINA QUE SOMA OU SUBTRAI TERMOS */
void expResFator (char* resposta); /* ROTINA QUE DIVIDE OU MULTIPLICA FATORES */
void expResFatorial (char* resposta); /* ROTINA QUE RESOLVE O FATORIAL DE UM FATOR */
void expResParenteses (char* resposta); /* ROTINA QUE RESOLVE UMA EXPRESSÃO DENTRO DE PARENTESES */
/* EM CONSTRUÇÃO */
void expAvalSinal (char* resposta); /* AVALIA + OU - UNÁRIO */ 
/* */
void atomo (char* resposta); /* DEVOLVE O VALOR NUMERICO DAS EXPRESSÕES POR EXTENSO*/
void pega_token (void);
void ajustaEXP (void);
int verificaProxToken (void);
int resPlural (int i, char** s); /* EM ORDENS COMPOSTAS, AVALIA TANTO A FORMA PLURAL QUANTO SINGULAR E ENFILA A FORMA INSERIDA */
void ajustaDelim (int* k, char* temp); /* AJUSTA DELIMITADORES COMPOSTOS COM HÍFEN ENTRE AS PALAVRAS */
void erroSS (int tipoErro); /* TODOS OS POSSÍVEIS ERROS (CHECAR lib/erros.txt) */
void criaIndices (FILE* in, Int2B** out, int size);

void filaInsere (int i, char* nome, char* valor);
void filaLibera (void);
int filaCount (void);

int analiSemantica (void);
int semUnidade (FilaNum** inicio);
int pegaOrdem (FilaNum* inicio);
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
    OPENFILE (dicionario, ARQ_ORDENS, "r");
    MALLOC (ref, sizeof(Ordem));
    _TEXP = EXP;
    NUMERO = token;
    pega_token ();
    if (!*token)
    {
        erroSS(3);
        return;
    }
    expResTermo (resposta);
    if (*token) erroSS (0);
    free (ref);
    fclose (dicionario);
}

void expResTermo (char* resposta)
{
    register char op;
    char* segTermo;
    expResFator (resposta);
    while ((op = *token) == '+' || op == '-')
    {
        pega_token ();
        MALLOC (segTermo, strlen(EXP));
        expResFator (segTermo);
        switch (op)
        {
            case '-':
            strcpy (resposta, subtrair (resposta, segTermo));
            free (segTermo);
            break;
            case '+':
            strcpy (resposta, soma (resposta, segTermo));
            free (segTermo);
            break;
        }
    }
}

void expResFator (char* resposta)
{
    register char op;
    char* segFator;
    expResFatorial (resposta);
    while ((op=*token) == '*' || op == '/')
    {
        pega_token ();
        MALLOC (segFator, strlen(EXP));
        expResFatorial (segFator);
        switch (op)
        {
            case '*':
            strcpy (resposta, multiplica (resposta, segFator));
            free (segFator);
            break;
            case '/':
            strcpy (resposta, divide (resposta, segFator));
            free (segFator);
            break;
        }
    }
}

void expResFatorial (char* resposta)
{
    char* proxFator;
    if (*token == '!')
    {
        pega_token ();
        MALLOC (proxFator, 300);
        expResParenteses (proxFator);
        char* temp = fatorial (proxFator);
        if (! temp) erroSS (8);
        strcpy (resposta, temp);
        free (proxFator);
        free (temp);
        if (!*resposta)
            erroSS (7);
        return;
    }
    expResParenteses (resposta);
}
/*
void expAvalSinal (char* resposta)
{
    register char op = 0;
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
*/

void expResParenteses (char* resposta)
{
    if (*token == '(')
    {
        pega_token ();
        expResTermo (resposta);
        if (*token != ')')
            erroSS (1);
        pega_token ();
    }
    else atomo (resposta);
}

void atomo (char* resposta)
{
    if (flagNUM == 1)
    {
        if (analiSemantica ())
        {
            char* temp = toNumber();
            strcpy (resposta, temp);
            filaLibera ();
            flagNUM = 0;
            pega_token ();
            return;
        }
        erroSS (3);
    }
    erroSS (0);
}

int analiSemantica (void)
{
    FilaNum* queueSem = queue;
    if (! queueSem) erroSS (3);
    if (pegaOrdem(queueSem) > DECILHAO || filaCount() > 43) erroSS (7); /* LIMITE DE DECILHÕES */
    char ord[2], i = 0;
    while (queueSem)
    {
        semUnidade (&queueSem);
        ord[i%2] = pegaOrdem (queueSem);
        if (i%2 && ord[0] <= ord[1]) erroSS (2);
        i++;
        if (queueSem) queueSem = queueSem -> prox;
    }
    return 1;
}

int semUnidade (FilaNum** inicio)
{
    FilaNum *fila = *inicio;
    while (fila && (fila -> classe < MIL || fila -> classe == CONJUCAO))
    {
        if (fila -> classe < VINTE && fila -> classe != DEZ)
        {
            if (fila -> prox)
            {
                if (fila -> prox -> classe == CONJUCAO) erroSS (11);
                if (fila -> prox -> classe < MIL) erroSS (2);
            }
        }
        else if (fila -> classe == DEZ || fila -> classe >= VINTE && fila -> classe <= NOVENTA)
        {
            if (fila -> prox && (fila -> prox -> classe < MIL || fila -> prox -> classe == CONJUCAO))
            {
                if (fila -> prox -> classe != CONJUCAO) erroSS (9);
                else if (fila -> prox -> prox == NULL) erroSS (10);
                else if (fila -> prox -> prox -> classe > NOVE) erroSS (2);
            }
        }
        else if (fila -> classe >= CEM && fila -> classe <= NOVECENTOS)
        {
            if (!strcmp (fila -> info -> nome, (char*) "cem") && (fila -> prox && fila -> prox -> classe == CONJUCAO)
            if (fila -> prox && (fila -> prox -> classe < MIL || fila -> prox -> classe == CONJUCAO))
            {
                if (fila -> prox -> classe != CONJUCAO) erroSS (9);
                else if (fila -> prox -> prox == NULL) erroSS (10);
                else if (fila -> prox -> prox -> classe > NOVENTA) erroSS (2);
            }
        }
        fila = fila -> prox;
    }
    *inicio = fila;
    return 1;
}

int pegaOrdem (FilaNum* inicio)
{
    FilaNum* aux = inicio;
    while (aux && (aux -> classe < MIL || aux -> classe == CONJUCAO)) aux = aux -> prox;
    if (! aux) return ZERO;
    return aux -> classe;
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
                //if (temp && *temp) free (temp);
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
    if (tipoErro%2 || ! tipoErro || tipoErro == 8 || tipoErro == 10 || tipoErro == 2)
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
    rewind (in);
}


void pega_token (void)
{
    rewind (dicionario);
    register char *temp;
    int i = 0, k = 0;
    char trade;
    temp = token;
    *temp = '\0';
    tipoToken = 0;
    if (!*EXP) return;
    while (isspace(*EXP)) ++EXP;
    while (EXP[k] && isalpha(EXP[k])) k++;
    trade = EXP[k];
    EXP[k] = '\0';
    ajustaDelim (&k, &trade);
    while (!feof (dicionario) && i < TAM*2)
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
                while (*EXP && (isalpha (*EXP)) || *EXP == ' ') EXP++;
                *EXP = trade;
                tipoToken = NUM;
                flagNUM = 1;
                filaInsere (i, ref->nome, ref->valor);
                rewind (dicionario);
                i=-1;
                if (verificaProxToken ()) return;
            }
            else if (strchr ("+-/*!e()", ref->valor[0]))
            {
                tipoToken = CONJUCAO;
                while (*EXP && (isalpha (*EXP) || *EXP == ' ' || *EXP == '-')) EXP++;
                NUMERO = temp;
                *temp++ = ref->valor[0];
                *temp = '\0';
                *EXP = trade;
                if (i != CONJUCAO)
                {
                    tipoToken = DELIMITADOR;
                    flagNUM = 0;
                    free (ref->nome);
                    free (ref->valor);
                    return;
                }
                else 
                {
                    filaInsere(i, ref->nome, ref->valor);
                    i=-1;
                    rewind (dicionario);
                }
            }
        }
        if (! flagNUM)
        {
            free (ref->nome);
            free (ref->valor);
        }
        else ajustaEXP();
        i++;
    }
    erroSS (0);
}

void ajustaEXP (void)
{
    while (*EXP && *EXP == ' ') EXP++;
    int k = strcspn (EXP, (char*) " ");
    EXP[k] = '\0';
}

int verificaProxToken (void)
{
    while (*EXP && *EXP == ' ') EXP++;
    char* temp = strpbrk (EXP, (char*) " ");
    if (! temp)
    {
        if (*EXP) return 0;
        return 1;
    }
    int k = temp - EXP;
    int i=0;
    char** dicT = (char**) malloc (sizeof (char*)*7);
    dicT[0] = (char*) "mais";
    dicT[1] = (char*) "menos";
    dicT[2] = (char*) "vezes";
    dicT[3] = (char*) "dividido";
    dicT[4] = (char*) "abre";
    dicT[5] = (char*) "fecha";
    dicT[6] = (char*) "fatorial";
    EXP[k] = '\0';
    for (i=0; i < 7; i++)
    {
        if (!strcmp (dicT[i], EXP))
        {
            free (dicT);
            EXP[k] = ' ';
            return 1;
        }
    }
    EXP[k] = ' ';
    free (dicT);
    return 0;
}

int resPlural (int i, char** s)
{
    char *nome = *s;
    if (! strchr ("mbtqscond", nome[0])) return 0;
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
    return fl;
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
        if (aux2 -> info) free (aux2 -> info);
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
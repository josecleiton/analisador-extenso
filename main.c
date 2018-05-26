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

/* PROX PASSO: CONVERSÃO DO RESULTADO PARA EXTENSO */

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
#define NUM_ERROS 12

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
    ZERO, UM, DOIS, TRES, QUATRO, CINCO, SEIS, SETE, OITO, NOVE, DEZ, ONZE, DOZE,
    TREZE, CATORZE, QUINZE, DEZESSEIS, DEZESSETE, DEZOITO, DEZENOVE, VINTE, TRINTA,
    QUARENTA, CINQUENTA, SESSENTA, SETENTA, OITENTA, NOVENTA, CEM, DUZENTOS,TREZENTOS,
    QUATROCENTOS, QUINHENTOS, SEISCENTOS, SETECENTOS, OITOCENTOS, NOVECENTOS, MIL,
    MILHAO, BILHAO, TRILHAO, QUATRILHAO, QUINTILHAO, SEXTILHAO, SETILHAO, OCTILHAO,
    NONILHAO, DECILHAO, CONJUCAO, NUM, DELIMITADOR
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
Int2B* ind;
FILE* dicionario;
FilaNum* queue;

char* expParsingStart (void); /* GATILHO DE PARTIDA */
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
int analiSemantica (void);
int semUnidade (FilaNum** inicio);
void pluralOrdem (FilaNum* inicio);
int pegaOrdem (FilaNum* inicio);
char* toNumber (void);
void toName (char** resposta);
int resMenorOrd (char** str, char* resultado, int* size);
void initString (char** s);
void filaInsere (int i, char* nome, char* valor);
void filaLibera (void);
int filaCount (void);

int main (void)
{
    EXP = expNum;
    char* resultado;
    scanf("%[^\n]", EXP);
    resultado = expParsingStart ();
    puts (resultado);
    return 0;
}

char* expParsingStart (void)
{
    char *resposta;
    MALLOC (resposta, 1024);
    OPENFILE (dicionario, ARQ_ORDENS, "r");
    MALLOC (ref, sizeof(Ordem));
    _TEXP = EXP;
    NUMERO = token;
    pega_token ();
    if (!*token)
    {
        erroSS(3);
        return NULL;
    }
    expResTermo (resposta);
    if (*token) erroSS (0);
    toName (&resposta);
    free (ref);
    fclose (dicionario);
    return resposta;
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
        pluralOrdem(queueSem);
        semUnidade (&queueSem);
        ord[i%2] = pegaOrdem (queueSem);
        if (i%2 && ord[0] <= ord[1]) erroSS (2);
        i++;
        if (queueSem) queueSem = queueSem -> prox;
    }
    return 1;
}

void pluralOrdem (FilaNum* inicio)
{
    FilaNum* aux = inicio;
    while (aux && (aux -> classe < MILHAO || aux -> classe == CONJUCAO)) aux = aux -> prox;
    if (aux && strstr (aux -> info -> nome, (char*) "oes"))
    {
        if (inicio -> classe != UM || inicio -> classe == CONJUCAO) return;
    }
    else if (!aux || inicio -> classe == UM || inicio -> classe == CONJUCAO) return;
    erroSS (12);
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
        else if (fila -> classe == DEZ || (fila -> classe >= VINTE && fila -> classe <= NOVENTA))
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
            if (!strcmp (fila -> info -> nome, (char*) "cem") && (fila -> prox && fila -> prox -> classe == CONJUCAO)) erroSS (12);
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
    char *resultado = NULL, *guardaClasse = NULL, *aux;
    initString (&resultado);
    initString (&guardaClasse);
    aux = guardaClasse;
    while (queue)
    {
        int k = queue -> classe;
        if (k!=CONJUCAO)
        {
            if (k<MIL)
            {
                guardaClasse = soma (queue->info->valor, guardaClasse);
            }
            else
            {
                char* temp = multiplica (queue->info->valor, guardaClasse);
                resultado = soma (temp, resultado);
                //if (temp && *temp) free (temp);
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
    free (aux);
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
    char strErro[512];
    int temp, i = 0, tamErro;
    Int2B *idc = NULL;
    criaIndices (erroS, &idc, NUM_ERROS);
    if (tipoErro%2 || ! tipoErro || tipoErro == 8 || tipoErro == 10 || tipoErro == 2 || tipoErro == 12)
    {
        fseek (erroS, idc[tipoErro], SEEK_SET);
        fgets (strErro, 512, erroS);
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
    MALLOC(ind, sizeof(Int2B)*(size+2));
    *ind = 0;
    char ch = getc (in);
    while (ch != EOF && i <= size)
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
                while ((*EXP && isalpha (*EXP)) || *EXP == ' ') EXP++;
                *EXP = trade;
                tipoToken = NUM;
                flagNUM = 1;
                filaInsere (i, ref->nome, ref->valor);
                rewind (dicionario);
                i=-1;
                if (verificaProxToken ()) return;
            }
            else if (strchr ("+/-*!e()", ref->valor[0]))
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

void toName (char** resposta)
{
    int tam = strlen (*resposta);
    if (tam > DECILHAO-10) return;
    char *resultado, *aux;
    Int2B ord, k=0, i;
    MALLOC (resultado, tam*200);
    criaIndices (dicionario, &ind, (TAM-4)*2);
    while (tam > 0)
    { //strcat (resultado, (char*) " e ");
        ord = (tam - 1)/3;
        resMenorOrd (resposta, resultado, &tam);
        fseek (dicionario, ind[ord-1+MIL], SEEK_SET);
        if (ord == 1)
        {
            MALLOC (aux, 5);
            fscanf (dicionario, "%[^=]", ++aux);
            *--aux = ' ';
            strcat (resultado, aux);
            free (aux);
        }
        if (**resposta) strcat (resultado, (char*) " e ");
    }
    strcpy (*resposta, resultado);
    free (resultado);
    free (ind);
}

int resMenorOrd (char** str, char* resultado, int* size)
{
    int tam = *size;
    int cond = tam%3;
    char label=0, *tmp, *s = *str, i = 3;
    STATE: while (*s && *s == '0' && i>0)
    {
        s++;
        cond = (cond-1)%3;
        i--;
    }
    if (cond<0) cond+=3;
    if (*s && *s != '0')
    {
        switch (cond)
        {
            case 0: flagNUM = CEM; tam-=3; cond = 3; break;
            case 2: flagNUM = VINTE; label--; tam-=2; break;
            case 1: flagNUM = UM; tam-=1; break;
        }
        while (cond)
        {
            label += *s - '0';
            if (cond == 2 && *s == '1')
            {
                label = 10;
                s++;
                label += *s - '0';
                cond = 1;
                flagNUM = UM;
            }
            fseek (dicionario, ind[label-1+flagNUM], SEEK_SET);
            MALLOC (tmp, 12);
            fscanf (dicionario, "%[^=]", tmp);
            if (strstr (tmp, (char*) "cem"))
            {
                strcpy (tmp, (char*) "cento");
                if (s[1] == '0' && s[2] == '0')
                {
                    strcpy (tmp, (char*) "cem");
                    s += 2;
                    cond = 1;
                }
            }
            strcat (resultado, tmp);
            if (cond != 1 && ((cond==3 && s[1] + s[2] != '0'+'0') || (cond==2 && s[1] != '0')))
                strcat (resultado, (char*) " e ");
            free (tmp);
            s++;
            if (cond != 1)
            {
                cond = (cond-1)%3;
                label = 0;
                i--;
                if (i<=0) break;
                if (tam<0) tam+=3;
                goto STATE;
            }
            break;
        }
    }
    while (*size - tam != 3) tam--;
    *str = s;
    *size = tam;
    return (tam-1)/3;
    //resDezena(s, resultado, strlen(s), cursor);
}

void resDezena (char* s, char* resultado, Int2B* size, Int2B* cursor)
{
    flagNUM = DEZ;
    char label, *tmp;

}

void resUnit (char* s, char* resultado, Int2B* size, Int2B* cursor)
{

}

/*
void resUnit (char* s, char* res, Int2B k, Int2B j)
{
    char digit = s[k-1] - '0';
    char w[12];
    memset (w, 0, 12);
    fseek (dicionario, ind[digit], SEEK_SET);
    fscanf (dicionario, "%[^=,]", w);
    strcat (res, w);
    res[strlen(res)] = ' ';
    s++;
}
*/
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

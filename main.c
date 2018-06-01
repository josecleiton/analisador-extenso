/*
    Universidade Estadual do Estado da Bahia - UNEB
    Por: José Cleiton Borges Correia
    Prof: Jorge Sampaio Farias
    Disciplina: Linguagem de Programação 1 - 2018.1
    #################################################
    | AVALIADOR DE EXPRESSÕES NUMERICAS POR EXTENSO |
    | Entrada: expressão numérica por extenso       |
    | Saída: expressão resolvida por extenso        |
    #################################################
*/

/* PROX PASSO: CORRIGIR CONVERSOR DE EXTENSO PARA NUMERO */

#ifndef INCLUSOS
    #define INCLUSOS
    #include "lib/preproc.h"
#endif
#include <time.h>
#include "lib/operacoes.c"

#define ARQ_DICT "lib/dicionario.cfg"
#define ARQ_ERROS "lib/erros.cfg"
#define ARQ_ENTRADA "lib/expressoes.txt"
#define ARQ_SAIDA "lib/resultados.txt"
#define ARQ_LOG "logs.txt"

#define CLEARBUF scanf ("%*c")
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
    Int2B classe;
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
char *EXP, *_TEXP, expNum[512], *expOut;
char token[2];
short int tipoToken, flagNUM, *ind; /* a partir daqui short int sera usado como Int2B (olhar typedef em preproc.h) */
FILE* dicionario;
FilaNum* queue;

int fileParsingInit (void);
char* expParsingStart (void); /* GATILHO DE PARTIDA */
void expResTermo (char* resposta); /* ROTINA QUE SOMA OU SUBTRAI TERMOS */
void expResFator (char* resposta); /* ROTINA QUE DIVIDE OU MULTIPLICA FATORES */
void expResFatorial (char* resposta); /* ROTINA QUE RESOLVE O FATORIAL DE UM FATOR */
void expResParenteses (char* resposta); /* ROTINA QUE RESOLVE UMA EXPRESSÃO DENTRO DE PARENTESES */
/* EM CONSTRUÇÃO void expAvalSinal (char* resposta);  AVALIA + OU - UNÁRIO */
void atomo (char* resposta); /* DEVOLVE O VALOR NUMERICO DAS EXPRESSÕES POR EXTENSO*/
void pega_token (void);
void ajustaEXP (void);
int verificaProxToken (void);
int resPlural (int i, char** s); /* EM ORDENS COMPOSTAS, AVALIA TANTO A FORMA PLURAL QUANTO SINGULAR E ENFILA A FORMA INSERIDA */
void ajustaDelim (int* k, char* temp); /* AJUSTA DELIMITADORES COMPOSTOS COM HÍFEN ENTRE AS PALAVRAS */
void erroSS (int tipoErro); /* TODOS OS POSSÍVEIS ERROS (CHECAR lib/erros.txt) */
void criaIndices (FILE* in, Int2B** out, int size, int del);
int analiSemantica (void);
int semUnidade (FilaNum** inicio);
void pluralOrdem (FilaNum* inicio);
Int2B pegaOrdem (FilaNum* inicio);
char* toNum (void);
void toName (char** resposta);
int toNameMenOrd (char** str, char* resultado, Int2B* size, Int2B* flagPlural);
void filaInsere (Int2B i, char* nome, char* valor);
FilaNum* pegaProxNum (FilaNum* inicio);
Int2B pegaProxClasse (FilaNum* inicio);
void filaLibera (void);
int filaCount (void);
int fstrcount (FILE* in);
void clearScreen (void);

int main (void)
{
    EXP = expNum;
    char* resultado, op;
    puts ("\n\t\tANALISADOR DE EXPRESSOES NUMERICAS POR EXTENSO\n");
    getchar (); CLEARBUF;
    while (1)
    {
        clearScreen();
        puts ("Selecione a entrada:\n a= Arquivo\n t= Teclado\n e= Sair\n\nopcao = ");
        scanf ("%c", &op);
        switch (op)
        {
            case 'a':
                clearScreen ();
                printf ("\tForam analisadas e resolvidas %d expressoes.\n\tOs resultados podem ser encontrados em %s\n", fileParsingInit (), ARQ_SAIDA);
                CLEARBUF;
                getchar (); CLEARBUF;
                break;
            case 't':
                clearScreen ();
                CLEARBUF;
                puts ("Digite uma expressao numerica: ");
                scanf ("%[^\n]", EXP);
                resultado = expParsingStart ();
                printf ("\n\tResultado: %s\n", resultado);
                getchar (); CLEARBUF;
                break;
            case 'e': return 0;
            default: 
                CLEARBUF;
                puts ("Opcao invalida.\n");
                getchar (); CLEARBUF;

        }
    }
    return 0;
}

int fileParsingInit (void)
{
    FILE* entrada;
    OPENFILE (entrada, ARQ_ENTRADA, "rb");
    FILE* saida;
    OPENFILE (saida, ARQ_SAIDA, "wb");
    int count = fstrcount (entrada), i = 0;
    Int2B* indices;
    criaIndices (entrada, &indices, count, '\n');
    while (count > 0)
    {
        fseek (entrada, indices[i++], SEEK_SET);
        fgets (EXP, 512, entrada);
        char* aux[2];
        aux[0] = strchr (EXP, '\n');
        aux[1] = strchr (EXP, '\r');
        if (aux[1]) aux[0] = aux[1];
        if (aux[0]) *(aux[0]) = '\0';
        expOut = expParsingStart ();
        fputs (expOut, saida);
        if (count-1)
            fputc ('\n', saida);
        fflush (saida);
        count--;
    }
    fclose (saida);
    fflush (stdout);
    fclose (entrada);
    return i;
}

char* expParsingStart (void)
{
    char *resposta;
    MALLOC (resposta, 1024);
    OPENFILE (dicionario, ARQ_DICT, "rb");
    MALLOC (ref, sizeof(Ordem));
    _TEXP = EXP;
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
        MALLOC (segTermo, 300);
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
        MALLOC (segFator, 300);
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
            strcpy (resposta, toNum());
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
    if (filaCount() > 43) erroSS (7); /* LIMITE DE DECILHÕES */
    Int2B ord[2], i = 0;
    while (queueSem)
    {
        pluralOrdem(queueSem);
        semUnidade (&queueSem);
        ord[i%2] = pegaOrdem (queueSem);
        if (i++%2 && ord[0] <= ord[1]) erroSS (2);
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
    int flag = 0;
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
        flag++;
    }
    *inicio = fila;
    if (! flag) erroSS (5);
    return 1;
}

Int2B pegaOrdem (FilaNum* inicio)
{
    FilaNum* aux = inicio;
    while (aux && (aux -> classe < MIL || aux -> classe == CONJUCAO)) aux = aux -> prox;
    if (! aux) return NOVECENTOS;
    return aux -> classe;
}

char* toNum (void)
{
    char *resultado, *aux, *ext;
    Int2B limit = pegaOrdem(queue), ord, proxOrd, proxClasse;
    Int2B i, flare = 0, flag;
    if (limit) limit = (limit+1-MIL)*3+3;
    else limit+=3;
    MALLOC (ext, limit*2+1);
    memset (ext, 0, limit*2+1);
    aux = ext;
    while (queue && limit)
    {
        i = queue -> classe;
        if (i != CONJUCAO && i < MIL)
        {
            ord = pegaOrdem (queue);
            if (i < DEZ)
            {
                if (! flare)
                {
                    strcpy (aux, (char*) "00");
                    aux += 2;
                    flare = 1;
                    flag = 0;
                }
                if (aux - ext && (*(aux-1) != '0' && *aux == '0' && *(aux+1) == '0')) aux++;
                *aux++ = *(queue -> info -> valor);
            }
            else if (i < CEM)
            {
                if (! flare)
                {
                    strcpy (aux++, (char*) "0");
                    flare = 1;
                    flag = 1;
                }
                strcpy (aux++, queue -> info -> valor);
            }
            else
            {
                strcpy (aux++, queue -> info -> valor);
                flare = 1;
                flag = 2;
            }
        }
        else if (i != CONJUCAO)
        {
            flare = 0;
            if (queue -> prox)
                proxOrd = pegaOrdem (queue -> prox);
            else proxOrd = NOVECENTOS;
            while (ord - proxOrd >= 1)
            {
                flare = 1;
                if (ord - proxOrd == 1)
                {
                    proxClasse = pegaProxClasse (queue -> prox);
                    if (proxClasse >= CEM)
                        break;
                    else if (proxClasse >= DEZ)
                    {
                        aux += flag;
                        *aux++ = '0';
                    }
                    else if (proxClasse >= UM)
                    {
                        strcat (aux, "00");
                        aux += 2 + flag;
                    }
                    else
                    {
                        strcat (aux, (char*) "000");
                        aux += 3 + flag;
                    }
                }
                else
                {
                    strcat (aux, (char*) "000");
                    aux += 3;
                }
                proxOrd++;
            }
        }
        queue = queue -> prox;
    }

    flare = strlen (ext);
    MALLOC (resultado, flare + 1);
    strcpy (resultado, ext);
    resultado[flare] = '\0';
    free (ext);
    while (*resultado == '0') resultado++;
    return resultado;
}


void erroSS (int tipoErro)
{
    FILE* erroS;
    OPENFILE (erroS, ARQ_ERROS, "rb");
    char strErro[512], *strBump;
    int temp, i = 0, tamErro, tamEXP;
    Int2B *idc = NULL;
    criaIndices (erroS, &idc, NUM_ERROS, '\n');
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
    strBump = &strErro[tamErro+i+1];
    for (tamEXP = strlen (_TEXP); tamEXP-temp; tamEXP--)
        *strBump++ = '~';
    *strBump++ = '\n';
    *strBump++ = '\0';
    char* toFile;
    Int2B size_toFile = strlen(strErro)+50;
    time_t now;
    struct tm * timeinfo;
    time (&now);
    timeinfo = localtime (&now);
    MALLOC (toFile, size_toFile);
    *toFile = '\0';
    strcpy (toFile, asctime(timeinfo));
    char* needle = strchr (toFile, '\n');
    *++needle = '\0';
    strcat (toFile, strErro);
    needle = strrchr (toFile, '\n');
    *++needle = '\0';
    FILE* logs;
    OPENFILE (logs, ARQ_LOG, "ab");
    fputs (toFile, logs);
    fflush (logs);
    fclose (logs);
    free (toFile);
    puts (strErro);
    free (idc);
    fclose (erroS);
    ERRO;
}

void criaIndices (FILE* in, Int2B** out, int size, int del)
{
    rewind (in);
    Int2B *ind, i = 1, k = 1;
    MALLOC(ind, sizeof(Int2B)*(size+2));
    *ind = 0;
    char ch = getc (in);
    while (ch != EOF && i <= size)
    {
        if (ch == del)
        {
            ind[i] = k;
            i++;
        }
        ch = getc (in);
        k++;
    }
    *out = ind;
    rewind (in);
}


void pega_token (void)
{
    rewind (dicionario);
    register char *temp;
    Int2B i = 0;
    int k = 0;
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
        MALLOC (ref->nome, TAM+DOZE);
        MALLOC (ref->valor, TAM+DOZE)
        fscanf (dicionario, "%[^=]=%[^\n]%*c", ref->nome, ref->valor);
        if (! strcmp (ref->nome, EXP) || resPlural(i, &ref->nome))
        {
            if (isdigit (ref->valor[0]))
            {
                *temp++ = ref -> valor[0];
                *temp = '\0';
                while (*EXP && (isalpha (*EXP) || *EXP == ' ')) EXP++;
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
    if (! strchr ("mbtqdscount", nome[0])) return 0;
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
    if (!**resposta)
    {
        strcpy (*resposta, (char*) "zero");
        return;
    }
    Int2B tam = strlen (*resposta);
    if (tam > DECILHAO-10) return;
    char *resultado, *aux = NULL;
    Int2B ord, plural;
    int flag;
    MALLOC (resultado, tam*20);
    memset (resultado, 0, tam*20);
    criaIndices (dicionario, &ind, (TAM-4)*2, '\n');
    while (tam > 0)
    {
        ord = (tam - 1)/3;
        flag = toNameMenOrd (resposta, resultado, &tam, &plural);
        tam = strlen (*resposta);
        fseek (dicionario, ind[ord-1+MIL], SEEK_SET);
        if (flag)
        {
            if (ord == 1)
            {
                MALLOC (aux, 5);
                fscanf (dicionario, "%[^=]", ++aux);
                *--aux = ' ';
                strcat (resultado, aux);
                free (aux);
            }
            else if (ord)
            {
                MALLOC (aux, 36);
                memset (aux, 0, 36);
                char* tmp = aux;
                fscanf (dicionario, "%[^=]", ++aux);
                char* del = strpbrk (aux, (char*) ",");
                aux[del - aux] = '\0';
                if (plural)
                {
                    *--del = '\0';
                    aux = del+2;
                }
                *--aux = ' ';
                strcat (resultado, aux);
                free (tmp);
            }
            if ((**resposta) && !((tam - 1)/3))
            {
                strcat (resultado, (char*) " e ");
                flagNUM = 0;
            }
            strcat (resultado, " ");
        }
        if (ord==1 && flagNUM)
        {
            Int2B AC = 0, c = 0;
            while ((*resposta)[c]) AC += (*resposta)[c++] - '0';
            if (AC) strcat (resultado, (char*) " e ");
        }
    }
    aux = strrchr (resultado, 'e');
    if (aux && (*(aux-1) == ' ' && *(aux+1) == ' ') && (*(aux+2) == ' ' || *(aux+2) == '\0') ) *aux = '\0';
    strcpy (*resposta, resultado);
    free (resultado);
    free (ind);
}

int toNameMenOrd (char** str, char* resultado, Int2B* size, Int2B* flagPlural)
{
    char *s = *str, label, *tmp;
    Int2B tam = *size, count = tam%3;
    if (! count) count += 3;
    const Int2B cnt = count;
    while (count)
    {
        label = 0;
        while (count && *s == '0')
        {
            count--;
            s++;
        }
        if (count)
        {
            switch (count)
            {
                case 1: flagNUM = UM; break;
                case 2: flagNUM = VINTE; label--; break;
                case 3: flagNUM = CEM; break;
            }
            if (count == 2 && *s == '1')
            {
                label = 10;
                s++;
                flagNUM = UM;
                count--;
            }
            label += *s - '0';
            fseek (dicionario, ind[label-1+flagNUM], SEEK_SET);
            MALLOC (tmp, 25);
            fscanf (dicionario, "%[^=]", tmp);
            if (strstr (tmp, (char*) "cem"))
            {
                strcpy (tmp, (char*) "cento");
                if (s[1] == '0' && s[2] == '0')
                {
                    strcpy (tmp, (char*) "cem");
                    s += 2;
                    count = 1;
                }
            }
            strcat (resultado, tmp);
            if (count != 1 && ((count==3 && s[1] + s[2] != '0'+'0') || (count==2 && s[1] != '0')))
                strcat (resultado, (char*) " e ");
            count--;
            s++;
            free (tmp);
        }
        else if (*s) tam = strlen (s);
    }
    *flagPlural = 1;
    if (*size == tam && *(s-1)=='1')  *flagPlural = 0;
    if (!*s) tam = 0;
    *str = s;
    if (strcmp (s, (char*) "000"))
    {
        tam -= cnt;
        *size = tam;
    }
    count = cnt;
    tam = 0;
    while (count)
    {
        tam += *(s-count) - '0';
        count--;
    }
    return (*s && tam);
}

void filaInsere (Int2B i, char* nome, char* valor)
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
    if (no -> classe < MIL)
        no -> ant = pegaProxNum (queue);
    else no -> ant = NULL;
    aux -> prox = no;
}

Int2B pegaProxClasse (FilaNum* inicio)
{
    Int2B classe;
    if (! inicio) return ZERO;
    while (inicio -> classe >= MIL) inicio = inicio -> prox;
    classe = inicio -> classe;
    return classe;
}

FilaNum* pegaProxNum (FilaNum* inicio)
{
    FilaNum* aux;
    while (inicio)
    {
        if (inicio -> classe < MIL) aux = inicio;
        inicio = inicio -> prox;
    }
    return aux;
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

int fstrcount (FILE* in)
{
    rewind (in);
    int i = 1;
    char ch = getc (in);
    while (ch != EOF)
    {
        if (ch == '\n') i++;
        ch = getc (in);
    }
    return i;
    rewind (in);
}

void clearScreen (void)
{
    int n;
    for (n = 0; n < 10; n++)
      printf ("\n\n\n\n\n\n\n\n\n\n");
}
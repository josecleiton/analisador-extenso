/*
**  Universidade Estadual do Estado da Bahia - UNEB
**  Por: José Cleiton Borges Correia
**  Prof: Jorge Sampaio Farias
**  Disciplina: Linguagem de Programação 1 - 2018.1
**  #################################################
**  | AVALIADOR DE EXPRESSÕES NUMERICAS POR EXTENSO |
**  | Entrada: expressão numérica por extenso       |
**  | Saída: expressão resolvida por extenso        |
**  #################################################
*/

#include "lib/main.h"
#include <time.h>
#include "lib/operacoes.c"
/* 
**   Vários tokens que auxiliam na análise (léxica/semântica)
**   Se esses termos não forem familiares, leia README.md
*/
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
**
**       VARIAVEIS GLOBAIS
**
*/
Ordem ref; /* struct ordem */
char *EXP; /* Ponteiro para expNum */
char *_TEXP; /* guarda a expressão sem modificações, para a possível exibição de erros */
char expNum[MAX_GEN]; /* Expressão que será analisada */
char token; /* guarda o token */
short tipoToken; /* sinalisa o tipo do token em analise */
unsigned flagNUM; /* sinaliza se o(s) token(s) em análise são numeros */
short *ind; /* vetor que guarda as posições das strings no ARQ_DICT */
FILE* dicionario;
FilaNum* queue;
/* OBS: a partir daqui short [int] será usado como SU (olhar typedef em lib/preproc.h) */

int main (void)
{
    EXP = expNum;
    char* resultado, op;
    puts ("\n\t\tANALISADOR DE EXPRESSOES NUMERICAS POR EXTENSO\n");
    CLRBUF;
    while(true)
    {
        clearScreen ();
        puts ("Selecione a entrada:\n a= Arquivo\n t= Teclado\n e= Sair\n\nopcao = ");
        scanf ("%c%*c", &op);
        switch (op)
        {
            case 'a':
                clearScreen ();
                printf ("\tForam analisadas e resolvidas %d expressoes.\n\tOs resultados podem ser encontrados em %s\n", fileParsingInit (), ARQ_SAIDA);
                printRes();
                CLRBUF;
                break;
            case 't':
                clearScreen ();
                puts ("Digite uma expressao numerica: ");
                scanf ("%[^\n]%*c", EXP);
                resultado = expParsingStart ();
                printf ("\n\tResultado: %s\n", resultado);
                free (_TEXP);
                CLRBUF;
                break;
            case 'e': return 0;
            default: 
                CLRBUF;
                puts ("Opcao invalida.\n");
        }
    }
}

int fileParsingInit (void)
{
    FILE* entrada;
    OPENFILE (entrada, ARQ_ENTRADA, "rb");
    FILE* saida;
    OPENFILE (saida, ARQ_SAIDA, "wt");
    int count = fstrcount (entrada), i = 0;
    char *expOut = NULL; /* Resultado da expressão analisada */
    SU* indices = criaIndices (entrada, count, '\n');
    while (count > 0)
    {
        fseek (entrada, indices[i++], SEEK_SET);
        fgets (EXP, MAX_GEN, entrada);
        char* aux[2];
        aux[0] = strchr (EXP, '\n');
        aux[1] = strchr (EXP, '\r');
        if (aux[1]) aux[0] = aux[1];
        if (aux[0]) *(aux[0]) = '\0';
        expOut = expParsingStart ();
        fputs (expOut, saida);
        fputc ('\n', saida);
        fflush (saida);
        free (_TEXP);
        count--;
    }
    free(indices);
    fclose (saida);
    fflush (stdout);
    fclose (entrada);
    return i;
}

void printRes(void)
{
	char ch = '\0';
	printf("\nDeseja visualizar todas as expressoes resolvidas? (S/N)\n");
	scanf("%c%*c", &ch);
	if(ch=='S' || ch=='s' || ch=='\n')
	{
		FILE* saida;
		OPENFILE (saida, ARQ_SAIDA, "rt");
		size_t s = maiorString(saida) + 1;
		char* handle = MALLOC(s);
		printf("\n\tRESULTADOS (uma expressao por linha):\n\n");
		while(fgets(handle,s,saida))
			printf("%s",handle);
		fclose(saida);
        free(handle);
	}
	else return;

}

size_t maiorString (FILE* stream)
{
	size_t k = 0;
	size_t maior = 1;
	char c = getc(stream);
	while(c!=EOF)
	{
		k++;
		if(c=='\n')
		{
			maior = (k>maior)?k:maior;
			k=0;
		}
		c=getc(stream);
	}
	rewind(stream);
	return maior;
}

char* expParsingStart (void)
{
    strToLower ();
    char *resposta = (char*) MALLOC (1024);
    char *fResposta = resposta;
    OPENFILE (dicionario, ARQ_DICT, "rb");
    ind = criaIndices (dicionario, TAM_DICT, '\n');
    _TEXP = EXP;
    pega_token ();
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
        pega_token ();
        segTermo = (char*) MALLOC (300);
        expResFator (segTermo);
        switch (op)
        {
            case '-':
            memswap(resposta, segTermo, subtrair);
            break;
            case '+':
            memswap(resposta, segTermo, soma);
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
        pega_token ();
        segFator = (char*) MALLOC (300);
        expResFatorial (segFator);
        switch (op)
        {
            case '*':
            memswap(resposta, segFator, multiplica);
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
        pega_token ();
        proxFator = (char*) MALLOC (300);
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
/* IMPLEMENTAR ISSO DEPOIS
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
    if (token == '(')
    {
        pega_token ();
        expResTermo (resposta);
        if (token != ')')   erroSS (1);
        pega_token ();
    }
    else atomo (resposta);
}

void atomo (char* resposta)
{
    if (flagNUM == true)
    {
        if (analiSemantica ())
        {
            char* toNumAnswer = toNum();
            strcpy (resposta, toNumAnswer);
            free(toNumAnswer);
            filaLibera ();
            flagNUM = false;
            pega_token ();
            return;
        }
        erroSS (3);
    }
    erroSS (0);
}

bool analiSemantica (void)
{
    FilaNum* queueSem = queue;
    if (! queueSem) erroSS (3);
    if (filaCount() > (DECILHAO-NOVECENTOS)*4-1) erroSS (7); /* LIMITE DE DECILHÕES */
    SU ord[2], i = 0;
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
    if (aux && strstr (aux -> info -> nome, (const char*) "oes"))
    {
        if (inicio -> classe != UM || inicio -> classe == CONJUCAO) return;
    }
    else if (!aux || inicio -> classe == UM || inicio -> classe == CONJUCAO) return;
    erroSS (12);
}

bool semUnidade (FilaNum** inicio)
{
    FilaNum *fila = *inicio;
    bool flag = false;
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
            if (!strcmp (fila -> info -> nome, (const char*) "cem") && (fila -> prox && fila -> prox -> classe == CONJUCAO)) erroSS (12);
            if (fila -> prox && (fila -> prox -> classe < MIL || fila -> prox -> classe == CONJUCAO))
            {
                if (fila -> prox -> classe != CONJUCAO) erroSS (9);
                else if (fila -> prox -> prox == NULL) erroSS (10);
                else if (fila -> prox -> prox -> classe > NOVENTA) erroSS (2);
            }
        }
        fila = fila -> prox;
        flag = true;
    }
    *inicio = fila;
    if (! flag) erroSS (5);
    return 1;
}

SU pegaOrdem (FilaNum* inicio)
{
    FilaNum* aux = inicio;
    while (aux && (aux -> classe < MIL || aux -> classe == CONJUCAO)) aux = aux -> prox;
    if (! aux) return NOVECENTOS;
    return aux -> classe;
}

char* toNum (void)
{
    char *resultado = NULL, *aux = NULL, *ext = NULL;
    FilaNum* queueHandle = queue;
    SU limit = pegaOrdem(queue), ord, proxOrd, proxClasse;
    SU i, flare = 0, flag;
    if (limit) limit = (limit+1-MIL)*3+3;
    else limit+=3;
    ext = (char*) MALLOC (limit*2+1);
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
                    strcpy (aux, (const char*) "00");
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
                    strcpy (aux++, (const char*) "0");
                    flare = 1;
                    /*flag = 1;*/
                }
                strcpy (aux++, queue -> info -> valor);
                flag = 1;
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
                    {
                        SU prevClass = queue -> ant -> classe;
                        if (prevClass >= CEM)
                            aux += 2;
                        else if (prevClass >= DEZ)
                            aux++;
                        break;
                    }
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
                        strcat (aux, (const char*) "000");
                        aux += 3 + flag;
                    }
                }
                else
                {
                    strcat (aux, (const char*) "000");
                    aux += 3;
                }
                proxOrd++;
            }
        }
        queue = queue -> prox;
    }

    flare = strlen (ext);
    resultado = (char*) MALLOC (flare + 1);
    strcpy (resultado, ext);
    resultado[flare] = '\0';
    free (ext);
    trataZeros (&resultado);
    queue = queueHandle;
    return resultado;
}


void erroSS (int tipoErro)
{
    FILE* erroS;
    OPENFILE (erroS, ARQ_ERROS, "rb");
    char strErro[MAX_GEN], *strBump;
    int temp, i = 0, tamErro, tamEXP;
    SU *idc = criaIndices (erroS, NUM_ERROS, '\n');
    fseek (erroS, idc[tipoErro], SEEK_SET);
    fgets (strErro, MAX_GEN, erroS);
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
    SU size_toFile = strlen(strErro)+50;
    time_t now;
    struct tm *timeinfo;
    time (&now);
    timeinfo = localtime (&now);
    toFile = (char*) MALLOC (size_toFile);
    *toFile = '\0';
    strcpy (toFile, asctime(timeinfo));
    char* needle = strchr (toFile, '\n');
    *++needle = '\0';
    strcat (toFile, strErro);
    needle = strrchr (toFile, '\n');
    *++needle = '\0';
    FILE* logs;
    OPENFILE (logs, ARQ_LOG, "at");
    fputs (toFile, logs);
    fflush (logs);
    fclose (logs);
    free (toFile);
    puts (strErro);
    free (idc);
    fclose (erroS);
    ERRO;
}

SU* criaIndices (FILE* in, int size, int del)
{
    rewind (in);
    SU *index = (SU*) MALLOC (sizeof(SU)*(size+2));
    SU i = 1, k = 1;
    *index = 0;
    char ch = getc (in);
    while (ch != EOF && i <= size)
    {
        if (ch == del)
            index[i++] = k;
        ch = getc (in);
        k++;
    }
    rewind (in);
    return index;
}


void pega_token (void)
{
    rewind (dicionario);
    SU i = 0;
    int k = 0;
    char trade = '\0';
    char valorTk = '\0';
    token = '\0';
    tipoToken = 0;
    if (!*EXP) return;
    while (isspace(*EXP)) ++EXP;
    while (EXP[k] && isalpha(EXP[k])) k++;
    trade = EXP[k];
    EXP[k] = '\0';
    ajustaDelim (&k, &trade);
    while (!feof (dicionario) && i < TAM_DICT)
    {
        fscanf (dicionario, "%[^=]=%[^\n]%*c", ref.nome, ref.valor);
        if (! strcmp (ref.nome, EXP) || resPlural(i, ref.nome))
        {
            valorTk = *(ref.valor);
            if (isdigit (valorTk))
            {
                token = valorTk;
                while (*EXP && (isalpha (*EXP) || *EXP == ' ')) EXP++;
                *EXP = trade;
                tipoToken = NUM;
                flagNUM = true;
                filaInsere (i, ref.nome, ref.valor);
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
                    filaInsere(i, ref.nome, ref.valor);
                    i = -1;
                    rewind (dicionario);
                }
            }
        }
        else ajustaEXP();
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

bool resPlural (int i, char *s)
{
    char *nome = s;
    if (! strchr ("mbtqdscount", *nome)) return 0;
    char* del = strchr (nome, ',');
    bool fl = false;
    if (!del) return 0;
    int k = del - nome;
    nome[k] = '\0';

    if (! strcmp (nome, EXP))
        fl = true;
    else
    {
        ++del;
        if (! strcmp (del, EXP))
        {
            char temp[MAXWLEN] = {'\0'};
            fl = true;
            strcpy (temp, del);
            strcpy (s, temp);
            s[++k] = '\0';
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

void toName (char** resposta)
{
    if (!**resposta)
    {
        strcpy (*resposta, (const char*) "zero");
        return;
    }
    SU tam = strlen (*resposta);
    if (tam > DECILHAO-10) return;
    char *resultado = (char*) MALLOC (tam*20);
    char *aux = NULL;
    SU ord, plural;
    int flag;
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
                aux = (char*) MALLOC (5);
                fscanf (dicionario, "%[^=]", ++aux);
                *--aux = ' ';
                strcat (resultado, aux);
                free (aux);
            }
            else if (ord)
            {
                aux = (char*) MALLOC (36);
                char* tmp = aux;
                fscanf (dicionario, "%[^=]", ++aux);
                char* del = strchr (aux, ',');
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
                strcat (resultado, (const char*) " e ");
                flagNUM = false;
            }
            strcat (resultado, " ");
        }
        if (ord==1 && flagNUM)
        {
            SU AC = 0, c = 0;
            while ((*resposta)[c]) AC += (*resposta)[c++] - '0';
            if (AC) strcat (resultado, (const char*) " e ");
        }
    }
    aux = strrchr (resultado, 'e');
    if (aux && (*(aux-1) == ' ' && *(aux+1) == ' ') && (*(aux+2) == ' ' || *(aux+2) == '\0') && (*(aux+3) == ' ' || *(aux+3) == '\0') ) *aux = '\0';
    strcpy (*resposta, resultado);
    free (resultado);
}

int toNameMenOrd (char** str, char* resultado, SU* size, SU* flagPlural)
{
    char *s = *str, label = 0, *tmp = NULL;
    SU tam = *size, count = tam%3;
    if (! count) count += 3;
    const SU cnt = count;
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
            tmp = (char*) MALLOC (25);
            fscanf (dicionario, "%[^=]", tmp);
            if (strstr (tmp, (const char*)"cem"))
            {
                strcpy (tmp, (const char*)"cento");
                if (s[1] == '0' && s[2] == '0')
                {
                    strcpy (tmp, (const char*) "cem");
                    s += 2;
                    count = 1;
                }
            }
            strcat (resultado, tmp);
            if (count != 1 && ((count==3 && s[1] + s[2] != '0'+'0') || (count==2 && s[1] != '0')))
                strcat (resultado, (const char*) " e ");
            count--;
            s++;
            free (tmp);
        }
        else if (*s) tam = strlen (s);
    }
    *flagPlural = (cnt == 1 && *(s-1) == '1') ? 0 : 1;
    if (!*s) tam = 0;
    *str = s;
    if (strcmp (s, (const char*) "000"))
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

void filaInsere (SU i, char* nome, char* valor)
{
    FilaNum *no = (FilaNum*) MALLOC (sizeof (FilaNum));
    FilaNum *aux = queue;
    no -> info = (Ordem*) MALLOC (sizeof (Ordem));
    strcpy (no->info->nome, nome);
    strcpy (no->info->valor, valor);
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
    no -> ant = pegaProxNum (queue);
    aux -> prox = no;
}

SU pegaProxClasse (FilaNum* inicio)
{
    SU classe = 0;
    if (! inicio) return classe;
    while (inicio -> classe >= MIL) inicio = inicio -> prox;
    classe = inicio -> classe;
    return classe;
}

FilaNum* pegaProxNum (FilaNum* inicio)
{
    FilaNum* aux = NULL;
    while (inicio)
    {
        if (inicio -> classe < MIL) aux = inicio;
        inicio = inicio -> prox;
    }
    return aux;
}

void filaLibera (void)
{
    FilaNum *aux = queue, *zombie = NULL;
    while (aux)
    {
        zombie = aux;
        aux = aux -> prox;
        if (zombie->info)
            free (zombie->info);
        free (zombie);
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

void strToLower (void)
{
    int i;
    for (i=0; EXP[i]; i++)
        if (isupper (EXP[i]))
            EXP[i] = tolower (EXP[i]);
}

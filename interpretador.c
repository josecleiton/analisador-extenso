#include "interpretador.h"
#include "operacoes.h"
#include "alloc.h"
#include <time.h>

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
uint16_t* ind; /* vetor que guarda as posições das strings no ARQ_DICT */
FILE* dicionario; /* lista de tokens */ 
ListaNum* list; /* guarda o número analisado por casas decimais em uma lista encadeada */

int interpretador (void)
{
    EXP = expNum;
    char* resultado, op;
    puts ("\n\t\tANALISADOR DE EXPRESSOES NUMERICAS POR EXTENSO\n");
    CLRBUF;
    while(true)
    {
        clearScreen ();
        puts ("Selecione a entrada:\n a= Arquivo\n t= Teclado\n h= Ajuda\n e= Sair\n\nopcao = ");
        scanf ("%c%*c", &op);
        switch (op)
        {
            case 'a':
                clearScreen ();
                printf ("\tForam analisadas e resolvidas %d expressoes.\n\tOs resultado podem ser encontrados em %s\n", fileParsingInit (), ARQ_SAIDA);
                printRes();
                CLRBUF;
                break;
            case 't':
                clearScreen ();
                puts ("Digite uma expressao numerica: ");
                scanf ("%[^\n]%*c", EXP);
                resultado = expParsingStart ();
                printf ("\nResultado: %s\n", resultado);
                free (_TEXP);
                CLRBUF;
                break;
            case 'h':
                clearScreen ();
                handBook ();
                CLRBUF;
                break;
            case 'e': return 0;
            default: 
                CLRBUF;
                puts ("Opcao invalida.\n");
        }
    }
}

void handBook (void)
{
    puts("A analise ocorre a partir de regras bem definidas, entao segue uma lista de comandos validos e instrucoes de uso:");
    puts("\n\nNUMEROS: \n");
    puts("A ordem e o plural importam. Nao utilize acentos nem pontuacao.");
    puts("dois milhoes = correto");
    puts("dois milhao = incorreto");
    puts("tres mil e quatro milhoes = incorreto");
    puts("quatro milhoes e tres mil = correto");
    puts("\n\nFORMATO DAS OPERACOES:\n");
    puts("Somar: numero mais numero");
    puts("Subtrair: numero menos numero");
    puts("Multiplicar: numero vezes numero");
    puts("Dividir: numero dividido por numero");
    puts("Resto: numero mod numero");
    puts("Exponencial: numero elevado a numero");
}

int fileParsingInit (void)
{
    FILE* entrada = openFile (ARQ_ENTRADA, "r");
    FILE* saida = openFile (ARQ_SAIDA, "wt");
    int i = 0;
    char *expOut = NULL; /* Resultado da expressão analisada */
    Index temp = criaIndices (entrada, false);
    uint16_t* indices = temp.index;
    int count = temp.tam;
    while (count > 0)
    {
        fseek (entrada, indices[i++], SEEK_SET);
        fgets (EXP, MAX_GEN, entrada);
        char* endline = strpbrk(EXP, "\r\n");
        if(endline) *endline = '\0';
        expOut = expParsingStart();
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
		FILE* saida = openFile (ARQ_SAIDA, "rt");
		size_t s = maiorString(saida) + 1;
		char* handle = (char*) alloc (s, sizeof (char));
		printf("\n\tRESULTADOS (uma expressao por linha):\n\n");
		while(fgets(handle,s,saida))
			printf("%s",handle);
		fclose(saida);
        free(handle);
	}
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

bool analiSemantica (void)
{
    ListaNum* listSem = list;
    if (! listSem) erroSS (3);
    if (listaCount() > (DECILHAO-NOVECENTOS)*4-1) erroSS (7); /* LIMITE DE DECILHÕES */
    uint16_t ord[2], i = 0;
    while (listSem)
    {
        pluralOrdem(listSem);
        semUnidade (&listSem);
        ord[i%2] = pegaOrdem (listSem);
        if (i++%2 && ord[0] <= ord[1]) erroSS (2);
        if (listSem) listSem = listSem -> prox;
    }
    return true;
}

void pluralOrdem (ListaNum* inicio)
{
    ListaNum* aux = inicio;
    while (aux && (aux -> classe < MILHAO || aux -> classe == CONJUCAO)) aux = aux -> prox;
    if (aux && strstr (aux -> info -> nome, (const char*) "oes"))
    {
        if (inicio -> classe != UM || inicio -> classe == CONJUCAO) return;
    }
    else if (!aux || inicio -> classe == UM || inicio -> classe == CONJUCAO) return;
    erroSS (12);
}

bool semUnidade (ListaNum** inicio)
{
    ListaNum *lista = *inicio;
    bool flag = false;
    while (lista && (lista -> classe < MIL || lista -> classe == CONJUCAO))
    {
        if (lista -> classe < VINTE && lista -> classe != DEZ)
        {
            if (lista -> prox)
            {
                if (lista -> prox -> classe == CONJUCAO) erroSS (11);
                if (lista -> prox -> classe < MIL) erroSS (2);
            }
        }
        else if (lista -> classe == DEZ || (lista -> classe >= VINTE && lista -> classe <= NOVENTA))
        {
            if (lista -> prox && (lista -> prox -> classe < MIL || lista -> prox -> classe == CONJUCAO))
            {
                if (lista -> prox -> classe != CONJUCAO) erroSS (9);
                else if (lista -> prox -> prox == NULL) erroSS (10);
                else if (lista -> prox -> prox -> classe > NOVE) erroSS (2);
            }
        }
        else if (lista -> classe >= CEM && lista -> classe <= NOVECENTOS)
        {
            if (!strcmp (lista -> info -> nome, (const char*) "cem") && (lista -> prox && lista -> prox -> classe == CONJUCAO)) erroSS (12);
            if (lista -> prox && (lista -> prox -> classe < MIL || lista -> prox -> classe == CONJUCAO))
            {
                if (lista -> prox -> classe != CONJUCAO) erroSS (9);
                else if (lista -> prox -> prox == NULL) erroSS (10);
                else if (lista -> prox -> prox -> classe > NOVENTA) erroSS (2);
            }
        }
        lista = lista -> prox;
        flag = true;
    }
    *inicio = lista;
    if (! flag) erroSS (5);
    return flag;
}

uint16_t pegaOrdem (ListaNum* inicio)
{
    ListaNum* aux = inicio;
    while (aux && (aux -> classe < MIL || aux -> classe == CONJUCAO)) aux = aux -> prox;
    if (! aux) return NOVECENTOS;
    return aux -> classe;
}

char* toNum (void)
{
    char *resultado = NULL, *aux = NULL, *ext = NULL;
    ListaNum* listHandle = list;
    uint16_t limit = pegaOrdem(list), ord, proxOrd, proxClasse;
    uint16_t i, flare = 0, flag;
    if (limit) limit = (limit+1-MIL)*3+3;
    else limit+=3;
    ext = (char*) alloc (limit*2+1, sizeof (char));
    aux = ext;
    while (list && limit)
    {
        i = list -> classe;
        if (i != CONJUCAO && i < MIL)
        {
            ord = pegaOrdem (list);
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
                *aux++ = *(list -> info -> valor);
            }
            else if (i < CEM)
            {
                if (! flare)
                {
                    strcpy (aux++, (const char*) "0");
                    flare = 1;
                    /*flag = 1;*/
                }
                strcpy (aux++, list -> info -> valor);
                flag = 1;
            }
            else
            {
                strcpy (aux++, list -> info -> valor);
                flare = 1;
                flag = 2;
            }
        }
        else if (i != CONJUCAO)
        {
            flare = 0;
            if (list -> prox)
                proxOrd = pegaOrdem (list -> prox);
            else proxOrd = NOVECENTOS;
            while (ord - proxOrd >= 1)
            {
                flare = 1;
                if (ord - proxOrd == 1)
                {
                    proxClasse = pegaProxClasse (list -> prox);
                    if (proxClasse >= CEM)
                    {
                        uint16_t prevClass = list -> ant -> classe;
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
        list = list -> prox;
    }

    flare = strlen (ext);
    resultado = (char*) alloc (flare + 1, sizeof (char));
    strcpy (resultado, ext);
    free (ext);
    trataZeros (&resultado);
    list = listHandle;
    return resultado;
}


void erroSS (int tipoErro)
{
    FILE* erroS = openFile (ARQ_ERROS, "rb");
    char strErro[MAX_GEN], *strBump;
    int temp, i = 0, tamErro, tamEXP;
    uint16_t *idc = criaIndices (erroS, NUM_ERROS).index;
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
    uint16_t size_toFile = strlen(strErro)+50;
    time_t now;
    struct tm *timeinfo;
    time (&now);
    timeinfo = localtime (&now);
    toFile = (char*) alloc (size_toFile, sizeof (char));
    strcpy (toFile, asctime(timeinfo));
    char* needle = strchr (toFile, '\n');
    *++needle = '\0';
    strcat (toFile, strErro);
    needle = strrchr (toFile, '\n');
    *++needle = '\0';
    FILE* logs = openFile (ARQ_LOG, "at");
    fputs (toFile, logs);
    fflush (logs);
    fclose (logs);
    free (toFile);
    puts (strErro);
    free (idc);
    fclose (erroS);
    abortWithLog(false);
}

Index criaIndices (FILE* in, int limite){
    Index resultado;
    char handle[MAX_GEN];
    bool rlloc = false;
    if(!limite){
        rlloc = true;
        limite = 32;
    }
    int i = 0;
    uint16_t* index = (uint16_t*) alloc(limite, sizeof (uint16_t));
    rewind(in);
    index[i++] = ftell(in);
    while(fgets(handle, MAX_GEN, in)){
        index[i++] = ftell(in);
        if(i == limite && rlloc){
            limite<<=1;
            index = (uint16_t*) realloc(index, sizeof (uint16_t)*limite);
            if(!index)
                abortWithLog(true);
        }        
    }
    index[--i] = 0;
    rewind(in);
    resultado.index = index;
    resultado.tam = i;
    return resultado;
}

uint16_t* _criaIndices (FILE* in, int size, int del)
{
    rewind (in);
    uint16_t *index = (uint16_t*) alloc (size+2, sizeof  (uint16_t));
    uint16_t i = 1, k = 1;
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

void toName (char** resposta)
{
    if (!**resposta)
    {
        strcpy (*resposta, (const char*) "zero");
        return;
    }
    uint16_t tam = strlen (*resposta);
    if (tam > DECILHAO-10) return;
    char *resultado = (char*) alloc (tam*20, sizeof (char));
    char *aux = NULL;
    uint16_t ord, plural;
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
                aux = (char*) alloc (5, sizeof (char));
                fscanf (dicionario, "%[^=]", ++aux);
                *--aux = ' ';
                strcat (resultado, aux);
                free (aux);
            }
            else if (ord)
            {
                aux = (char*) alloc (36, sizeof (char));
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
            uint16_t AC = 0, c = 0;
            while ((*resposta)[c]) AC += (*resposta)[c++] - '0';
            if (AC) strcat (resultado, (const char*) " e ");
        }
    }
    aux = strrchr (resultado, 'e');
    if (aux && (*(aux-1) == ' ' && *(aux+1) == ' ') && (*(aux+2) == ' ' || *(aux+2) == '\0') && (*(aux+3) == ' ' || *(aux+3) == '\0') ) *aux = '\0';
    strcpy (*resposta, resultado);
    free (resultado);
}

int toNameMenOrd (char** numberInput, char* resultado, uint16_t* size, uint16_t* flagPlural)
{
    char *currentNumber = *numberInput, label = 0, *tmp = NULL;
    uint16_t tam = *size, count = tam%3;
    if (! count) count += 3;
    const uint16_t cnt = count;
    while (count)
    {
        label = 0;
        while (count && *currentNumber == '0')
        {
            count--;
            currentNumber++;
        }
        if (count)
        {
            switch (count)
            {
                case 1: flagNUM = UM; break;
                case 2: flagNUM = VINTE; label--; break;
                case 3: flagNUM = CEM; break;
            }
            if (count == 2 && *currentNumber == '1')
            {
                label = 10;
                currentNumber++;
                flagNUM = UM;
                count--;
            }
            label += *currentNumber - '0';
            fseek (dicionario, ind[label-1+flagNUM], SEEK_SET);
            tmp = (char*) alloc (25, sizeof (char));
            fscanf (dicionario, "%[^=]", tmp);
            if (strstr (tmp, (const char*)"cem"))
            {
                strcpy (tmp, (const char*)"cento");
                if (currentNumber[1] == '0' && currentNumber[2] == '0')
                {
                    strcpy (tmp, (const char*) "cem");
                    currentNumber += 2;
                    count = 1;
                }
            }
            strcat (resultado, tmp);
            if (count != 1 && ((count==3 && currentNumber[1] + currentNumber[2] != '0'+'0') || (count==2 && currentNumber[1] != '0')))
                strcat (resultado, (const char*) " e ");
            count--;
            currentNumber++;
            free (tmp);
        }
        else if (*currentNumber) tam = strlen (currentNumber);
    }
    *flagPlural = (cnt == 1 && *(currentNumber-1) == '1') ? 0 : 1;
    if (!*currentNumber) tam = 0;
    *numberInput = currentNumber;
    if (strcmp (currentNumber, (const char*) "000"))
    {
        tam -= cnt;
        *size = tam;
    }
    count = cnt;
    tam = 0;
    while (count)
    {
        tam += *(currentNumber-count) - '0';
        count--;
    }
    return (*currentNumber && tam);
}

void listaInsere (uint16_t i, char* nome, char* valor)
{
    ListaNum *no = (ListaNum*) alloc (1, sizeof (ListaNum));
    ListaNum *aux = list;
    no -> info = (Ordem*) alloc (1, sizeof (Ordem));
    strcpy (no->info->nome, nome);
    strcpy (no->info->valor, valor);
    no -> classe = i;
    no -> prox = NULL;
    if (! list)
    {
        list = no;
        no -> ant = NULL;
        return;
    }
    while (aux && aux->prox)
        aux = aux -> prox;
    no -> ant = pegaProxNum (list);
    aux -> prox = no;
}

uint16_t pegaProxClasse (ListaNum* inicio)
{
    uint16_t classe = 0;
    if (! inicio) return classe;
    while (inicio -> classe >= MIL) inicio = inicio -> prox;
    classe = inicio -> classe;
    return classe;
}

ListaNum* pegaProxNum (ListaNum* inicio)
{
    ListaNum* aux = NULL;
    while (inicio)
    {
        if (inicio -> classe < MIL) aux = inicio;
        inicio = inicio -> prox;
    }
    return aux;
}

void listaLibera (void)
{
    ListaNum *aux = list, *zombie = NULL;
    while (aux)
    {
        zombie = aux;
        aux = aux -> prox;
        if (zombie->info)
            free (zombie->info);
        free (zombie);
    }
    list = NULL;
}

int listaCount (void)
{
    ListaNum* aux;
    int n;
    for (n = 0, aux = list; aux; aux = aux -> prox, n++);
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
    int i = 0;
    while(EXP[i]){
        EXP[i] = tolower (EXP[i]);
        i++;
    }
}


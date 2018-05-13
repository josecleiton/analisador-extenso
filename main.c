/*
    BY: José Cleiton Borges Correia
    Prof: Jorge Sampaio Farias
    Disciplina: Linguagem de Programação 1 - 2018.1
    ESTRUTURA DO ANALISADOR:
    resultado := expressão númerica por extenso
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

typedef struct ordem Ordem;
typedef struct ocorre Ocorre;
struct ordem
{
    char* nome;
    char* valor;
};
struct ocorre
{
    char pos;
    char cursor;
};
enum tokens
{
    UNIT,
    DEZ = 20,
    CENT = 28,
    MIL = 37,
    MILHAO,
    BILHAO,
    TRILHAO,
    QUATRILHAO,
    QUINTILHAO,
    SEXTILHAO,
    CONJUCAO,
    ABRE_P,
    FECHA_P,
    SOMA,
    SUBTRACAO,
    MULTI,
    DIV,
    FATORIAL,
    DELIMITADOR /* 52 */
};

/* 
*
*
*       VARIAVEIS GLOBAIS 
*
*
*/
Ordem* ref;
char *EXP, *_TEXP, *NUMERO, *FIMNUMERO;
char token[30], *token_tmp;
char tipoToken;
char strErro[300];

void erroSintaxe (int tipoErro);
void expParsingStart (char resultado[]);
void expResTerms (char resultado[]); /* ROTINA QUE SOMA OU SUBTRAI TERMOS */
void expResFator (char resultado[]);
void expResFatorial (char resultado[]);
void expResParenteses (char resultado[]);
void atomo (char resultado[]);
char* get_token (void);
void getNumber (char resultado[]);
Ordem* cria_dic (void);
int compara (char* s1, char* s2);
char* cardinalNumeral (char* card);

int main (void)
{
    EXP = (char*) malloc (sizeof (TAM*10));
    char* resultado;
    scanf("%[^\n]", EXP);
    resultado = (char*) malloc(strlen(EXP)*2);
    expParsingStart (resultado);
    puts (resultado);
    return 0;
}

Ordem* cria_dic (void)
{
    FILE* nomes = fopen (ARQ_ORDENS, "rt");
    if (! nomes)
    {
        printf("Arquivo não encontrado!\nFinalizando o programa...\n");
        ERRO;
    }
    Ordem* ref = (Ordem*) malloc (sizeof(Ordem)*TAM*2);
    char i = 0;
    if (! ref) ERRO;

    while (! feof(nomes))
    {
        ref[i].nome = (char*) malloc (TAM);
        ref[i].valor = (char*) malloc (TAM);
        if (! ref[i].nome || ! ref[i].valor) ERRO;
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
    tipoToken = 0;
    getNumber (resultado);
}

void getNumber (char resultado[])
{
    register char *temp;
    int count = 0;
    if (!tipoToken) temp = NUMERO = token;
    while (tipoToken != DELIMITADOR)
    {
        if (count)
        { 
            strcat (resultado, (char*) "-");
            temp = get_token ();
        }
        else get_token ();
        if (!tipoToken) break;
        strcat (resultado, temp);
        count++;
    }
    FIMNUMERO = EXP-2;
    NUMERO = EXP;
    if (!*token)
    {
        erroSintaxe (3);
        return;
    }
    expResTerms (resultado);
}

void expResTerms (char resultado[])
{
    register char op = *token;
    char* temp;
    expResFator (resultado);
    while (op == '+' || op == '-')
    {
        getNumber (temp);
        expResFator (temp);
        switch (op)
        {
            case '-':
            subtrair (resultado, temp);
            break;
            case '+':
            soma (resultado, temp);
            break;
        }
    }
}

void expResFator (char resultado[])
{
    register char op = *token;
    char* temp;
    expResFatorial (resultado);
}

void expResFatorial (char resultado[])
{
    char* temp;
    expResParenteses (resultado);
    if (*token == '!')
    {
        getNumber (temp);
        if (*temp == '-')
        {
            erroSintaxe (4);
            return;
        }
    }
}

void expResParenteses (char resultado[])
{
    char* temp;
    if (*token == '(')
    {
        getNumber (resultado);
        expResTerms (resultado);
        if (*token != ')')
            erroSintaxe (1);
        getNumber (temp);
    }
    else atomo (resultado);
}

void atomo (char resultado[])
{
    if (tipoToken >= UNIT && tipoToken <= SEXTILHAO)
    {
        resultado = cardinalNumeral (token);
        get_token ();
        return;
    }
    erroSintaxe (0);
}

char* get_token (void)
{
    register char *temp;
    int i;
    tipoToken = 0;
    temp = NUMERO;
    *temp = '\0';
    if (!*EXP) return NULL;
    while (isspace (*EXP)) 
        ++EXP;
    int k = 0;
    char chEXP;
    while (EXP[k] && isalpha (EXP[k])) k++;
    chEXP = EXP[k];
    EXP[k] = '\0';
    for (i=0; i<TAM*2; i++)
    {
        if (! compara (EXP, ref[i].nome)) /*SE ELE FOR UM NUMERO*/
        {
            int j = 0;
            if (isdigit (ref[i].valor[0]))
            {
                tipoToken = i;
                while (*EXP && (isalpha (*EXP))) EXP++;
                strcat (temp, ref[i].nome);
                *EXP = chEXP;
                /* PROCURAR FUNÇÃO QUE POSICIONE O PONTEIRO TEMP APÓS A ULTIMA LETRA DE REF[I].NOME */
                NUMERO = temp + 1 +  strlen (ref[i].nome);
                /*temp[strlen(ref[i].nome)] = '\0';*/
                if (!*NUMERO) return temp;
                return NUMERO;
            }
            else if (strchr("()+-*/!e", ref[i].valor[0]))
            {
                tipoToken = CONJUCAO;
                while (*EXP && (isalpha (*EXP) || isspace (*EXP)))
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

int compara (char* s1, char* s2) /*relativo a strcmp mas para quando encontra o espaço*/
{
	int i;
	for(i=0; (s1[i]!='\0' && s1[i]!=' ') && s2[i]!='\0'; ++i)
	{
		int d = s1[i] - s2[i];
		if (d != 0)
			return d;
	}
	if (s1[i]==' ') return 0;
    return s1[i]-s2[i];
}

void erroSintaxe (int tipoErro)
{
    FILE* erroS = fopen (ARQ_ERROS, "r");
    int temp, i = 0, tamErro;
    if (! tipoErro)
    {
        fscanf (erroS, "%s\n\t", strErro);
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
    else if (tipoErro == 1)
    {

    }
    else if (tipoErro == 2)
    {

    }
    else if (tipoErro == 3)
    {

        fseek (erroS, 175, SEEK_SET);
        fscanf (erroS, "%s*c", strErro);
    }
    puts (strErro);
    fclose (erroS);
}
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
    ABRE_P,
    FECHA_P,
    CONJUCAO,
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
char token[30];
char tipoToken;
char strErro[300];

void erroSintaxe (int tipoErro);
void expParsingStart (char resultado[]);
void expResTerms (char resultado[]); /* ROTINA QUE SOMA OU SUBTRAI TERMOS */
void expResFator (char resultado[]);
void atomo (char resultado[]);
void get_token (void);
void getNumber (char resultado[]);
Ordem* cria_dic (void);
int compara (char* s1, char* s2);
char* cardinalNumeral (char* card);

int main (void)
{
    EXP = (char*) malloc (sizeof (TAM*10));
    char* resultado;
    scanf("%[^\n]", EXP);
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
    register char *temp = token;
    if (!tipoToken) NUMERO = EXP;
    while (tipoToken != DELIMITADOR)
    {
        get_token ();
        strcat (resultado, (char*) "-");
        strcat (resultado, temp);
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
        get_token();
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
    register char op;
    char* temp;
}

void expResFatorial (char resultado[])
{

}

void expResParenteses (char resultado[])
{
    if (*token == '(')
    {
        get_token ();
        expResTerms (resultado);
        if (*token != ')')
            erroSintaxe (1);
        get_token ();
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

void get_token (void)
{
    register char *temp;
    int i;
    tipoToken = 0;
    temp = token;
    *temp = '\0';
    if (!*EXP) return;
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
                while (*EXP && (isalpha (*EXP) || isspace (*EXP)))
                    EXP++;
                strcat (temp, ref[i].nome);
                /*temp[strlen(ref[i].nome)] = '\0';*/
                break;
            }
            else if (strchr("()+-*/!", ref[i].valor[0]))
            {
                tipoToken = DELIMITADOR;
                while (*EXP && (isalpha (*EXP) || isspace (*EXP)))
                {
                    EXP++;
                }
                *temp++ = ref[i].valor[0];
                *temp = '\0';
                break;
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

char* cardinalNumeral (char* card)
{
    int cursor = 0, i = 0, k;
    char* flag = NULL;
    char* num = (char*) malloc (strlen(card));
    return num;
}

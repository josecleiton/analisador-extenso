#ifndef INCLUSOS
    #define INCLUSOS
    #include "lib/preproc.h"
#endif
#define TAM 26
#define ARQ_ORDENS "lib/ordens.txt"
typedef struct ordem Ordem;
struct ordem
{
    char* nome;
    char* valor;
};
enum tokens
{
    UNIT,
    DEZ = 21,
    CENT = 29,
    MIL = 38,
    MILHAO,
    BILHAO,
    TRILHAO,
    QUATILHAO,
    QUINTILHAO,
    SEXTILHAO,
    ABRE_P,
    FECHA_P,
    SOMA,
    SUBTRACAO,
    MULTI,
    DIV,
    MOD,
    FATORIAL
};
/*
*   FUNÇÃO QUE DEVOLVE O "DICIONÁRIO" PARA
*   A ANÁLISE DOS NUMEROS PRESENTES DE FORMA 
*   EXTENSA NAS EXPRESSÕES
*/
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
    return ref;
}

int compara (char* s1, char* s2) //relativo a strcmp
{
	int i;
	for(i=0; (s1[i]!='\0' && s1[i]!=' ') && s2[i]!='\0'; ++i)
	{
		int d = s1[i] - s2[i];
		if (d != 0)
			return d;
	}
	return 0;
}

char* cardinalNumeral (char a[])
{
    Ordem* ref = cria_dic ();
    int cursor = 0, i = 0;
    char* flag = NULL;
    char* num = (char*) malloc (strlen(a));
    while(a[cursor]!='\0') // PROCURAR FUNCAO DA STRING.H QUE FAÇA ISSO
    {
        if (! isspace (a[cursor])) 
        {
            i=0;
            while (i<TAM*2)
            {
                if (! compara(&a[cursor], ref[i].nome))
                {
                    break;
                }
                i++;
            }
            if (i == TAM*2) return (char*) "Expressao invalida";
        }
        flag = strpbrk (&a[cursor], " ");
        if (! flag) break;
        cursor = flag + 1 - a;
    }
    return (char*) "";
}

int main (void)
{
    char EXP[TAM*10];
    scanf("%[^\n]", EXP);
    char* res = cardinalNumeral (EXP);
    puts (res);
    return 0;
}
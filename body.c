#ifndef INCLUSOS
    #define INCLUSOS
    #include "lib/preproc.h"
#endif
#define TAM 26
#define ARQ_ORDENS "lib/ordens.txt"
#define ARQ_SAIDA "lib/resultados.txt"
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
    SOMA,
    SUBTRACAO,
    MULTI,
    DIV,
    MOD,
    FATORIAL /* 52 */
};
char flagErro;
Ocorre flagPos[7] = {0, 0, 0, 0, 0, 0, 0}; /*2 posições para os parenteses | 4 posições para as ordens | 1 posição para a operação */
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

char* erroSintaxe (char* a, int cursor)
{
    char* erro = (char*) malloc (strlen(a)+120+cursor);
    if (!erro) ERRO;
    strcpy (erro, "erro de sintaxe: ");
    switch (flagErro)
    {
        case 1:
        strcat (erro, "palavra nao faz parte de uma expressao numerica valida.\n\t");
        break;
        
        case 2:
        strcat (erro, "alguma abertura ou fechamento de parentese esta faltando.\n\t");
        break;
    }
    strcat (erro, a);
    strcat (erro, "\n\t");
    int tamErro = strlen(erro), i = 0;
    while (i<cursor)
    {
        erro[tamErro+i] = ' ';
        i++;
    }
    erro[tamErro+i] = '^';
    erro[tamErro+i+1] = '\n';
    return erro;
}

void remEspacosInicio (char* str) /*SE OS ESPAÇO VIEREM NO COMEÇO, COPIE TODA A STRING PARA O COMEÇO*/
{
    int i, sp = 0;
    if (isspace (str[0]))
    {
        while (isspace (str[sp+1]))
            sp++;
        sp++;
        for (i=0; str[sp]!='\0'; i++)
        {
            str[i] = str[sp];
            sp++;
        }
        str[i] = '\0';
    }
}

char* remEspacos (char* str) /* SE HOUVEREM ESPAÇOS NO MEIO E NO FIM, RETIRE-OS */
{
    char* vazio = (char*) malloc (strlen(str));
    int i = 0, j = 0;
    
    while (str[i] != '\0') 
    {
        if (str[i] == ' ') 
        {
            int temp = i + 1;
            if (str[temp] != '\0') 
            {
                while (str[temp] == ' ') 
                {
                    i++;
                    temp++;
                }
            }
        }
        vazio[j] = str[i];
        i++;
        j++;
    }
    free (str);
    vazio[j] = '\0';
    return vazio;
 
}

char* cardinalNumeral (char* exps)
{
    Ordem* ref = cria_dic ();
    int cursor = 0, i = 0, k;
    char* flag = NULL;
    remEspacosInicio (exps);
    char* a = remEspacos (exps);
    char* num = (char*) malloc (strlen(a));

    while(a[cursor]!='\0')
    {
        i=0;
        while (i<TAM*2)
        {
            if (! compara (&a[cursor], (char*) "abre ")) 
            {        
                a[cursor+4] = '-';
                flagPos[0].pos++;
                flagPos[0].cursor = cursor;
            }
            else if (! compara (&a[cursor], (char*) "fecha "))
            {
                a[cursor+5] = '-';
                flagPos[1].pos++;
                flagPos[1].cursor = cursor;
            }  
            if (! compara(&a[cursor], ref[i].nome))
            {

                if (i>=UNIT && i<DEZ)
                {
                    flagPos[2].pos++;
                    flagPos[2].cursor = cursor;
                }
                else if (i>=DEZ && i<CENT)
                {
                    flagPos[3].pos++;
                    flagPos[3].cursor = cursor;
                }
                else if (i>=CENT && i<MIL)
                {
                    flagPos[4].pos++;
                    flagPos[4].cursor = cursor;
                }
                else if (i>=MIL && i<ABRE_P)
                {
                    flagPos[5].pos++;
                    flagPos[5].cursor = cursor;
                }
                strcat (num, ref[i].valor);
                break;
            }
            i++;
        }



        for (k=0; k<6; k++)
        {
            if (flagPos[k].pos > 1)
            {
                flagErro = 3;
                char* erro = (char*) malloc (500);
                if (!erro) ERRO;
                strcpy (erro, erroSintaxe (a, (int) flagPos[k].cursor));
            }
        }

        if (i == TAM*2)
        {
            flagErro = 1;
            char* erro = (char*) malloc (500);
            if (!erro) ERRO;
            strcpy (erro, erroSintaxe (a, cursor));
            puts (erro);
            return erro;
        }

        flag = strpbrk (&a[cursor], " ");
        if (! flag) break;

        cursor = flag + 1 - a;
    }

    if (flagPos[0].pos != flagPos[1].pos)
    {
        flagErro = 2;
        char* erro = (char*) malloc (500);
        if (!erro) ERRO;
        strcpy (erro, erroSintaxe (a, 0));
        puts (erro);
        return erro;
    }

    return num;
}

int main (void)
{
    char* EXP = (char*) malloc (sizeof (TAM*10));
    scanf("%[^\n]", EXP);
    char* res = cardinalNumeral (EXP);
    puts (res);
    return 0;
}
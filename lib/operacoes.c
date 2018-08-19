#ifndef INCLUSOS
    #define INCLUSOS
    #include "preproc.h"
#endif
#include <math.h>
#define MAX 10000

char* soma (char op1[], char op2[]);
char* subtrair (char a[], char b[]);
char* completaMenor (char a[], char b[], char* menor);
char* multiplica (char a[], char b[]);
char* unsigneDiv (char a[], char D[], bool MOD);
char* unExpo (char a[], char b[]);
char* fatorial (char a[]);
int strIsDigit (char a[]);
int fatorial_multiplicador (int a, char fat[], int limit);
bool char2int (char a[]);
bool int2char (char a[], int tam);
bool inverte (char a[]); /* strrev (apenas para Janelas OS) */
int maior (int a, int b);
int menor (int a, int b);
bool strCmpNum (char x[], char b[]);
int numDigitos (int x);
void trataZeros (char** K);

/*
**  GERENCIA MEMORIA ENTRE O RESULTADO E OS OPERADORES DAS FUNÇÕES
*/

bool swap(char a[], char b[], char* (*f)(char*, char*)) {
    char* temp = f(a, b);
    strcpy(a, temp);
    free(temp);
    return (a != NULL);
}

bool swapDiv(char a[], char b[], bool mod, char* (*f)(char*, char*, bool)) {
    char* temp = f(a, b, mod);
    strcpy(a, temp);
    free(temp);
    return (a != NULL);
}

/*
int main (void)
{
    char a[MAX];
    char b[MAX];
    char *c;
    printf("Digite o primeiro numero: ");
    scanf("%s",a);
    printf("Digite o segundo numero: ");
    scanf("%s",b);
    printf("div dos dois numeros: ");
    c = unsigneDiv (a, b, 0);
    puts (c);
    return 0;
}
*/
bool inverte (char a[])
{
    int i, tam = strlen (a);
    char c;
    for (i=0; i<tam/2; i++)
    {
        c = a[i];
        a[i] = a[tam-1-i];
        a[tam-1-i] = c;
    }
    return true;
}

bool char2int (char a[])
{
    int i, tam = strlen (a);
    for (i=0; i<=tam; i++)
        a[i] = a[i] - '0';
    return true;
}

bool int2char (char a[], int tam)
{
    int i;
    for (i=0; i<tam-1; i++)
    {
        a[i] += '0';
    }
    if (! isdigit(a[i-1])) a[i-1] = '\0'; 
    a[i] = '\0';
    return true;
}

int maior (int a, int b)
{
    return (a>=b)*a + (b>a)*b;
}

int menor (int a, int b)
{
    return (a<=b)*a + (b<a)*b;
}

int strIsDigit (char a[])
{
    int i = 0;
    while (a[i])
    {
        if (!isdigit (a[i]) && !(a[i] >= 0 && a[i] <= 9)) 
            return 0;
        i++;
    }
    return i;
}

char* soma (char a[], char b[])
{
    int ta = strlen (a), tb = strlen (b), ts = maior (ta, tb);
    if (! ta)
        return b;
    else if (! tb)
        return a;
    int i, vaium = 0;
    char *soma, *Op2, *Op1, flag = 0;
    Op1 = (ta >= tb) ? a : b;
    Op2 = completaMenor (a, b, &flag);
    if (! Op2)
        Op2 = (ta >= tb) ? b : a;
    soma = (char*) MALLOC (ts+2);
    memset (soma, 0, ts+2);
    *soma = '0';
    soma++;
    for (i = ts-1; i >= 0; i--)
    {
        soma[i] = (Op1[i]-'0') + (Op2[i]-'0') + vaium;
        vaium = 0;
        if (soma[i] >= 10)
        {
            soma[i] %= 10;
            vaium = 1;
        }
        soma[i] += '0';
    }
    *--soma += vaium;
    trataZeros (&soma);
    return soma;
}

 char* subtrair (char a[], char b[])
 {
    char *min = NULL, *subt = NULL, flagSinal, flagMenor; /* flagMenor = [ se menor == 1, então a string a é menor; se menor == 0, então string a é maior; se menor == -1, ambas têm o mesmo tamanho ] */
    int i;

    int tamMinuendo = strlen(a), tamSubtraendo = strlen(b);
    if (! tamMinuendo)
        return b;
    else if (! tamSubtraendo)
        return a;
    if (tamMinuendo != tamSubtraendo)
    {
        subt = completaMenor (a, b, &flagMenor);
        if (flagMenor)
        {
            min = b;
            flagSinal = 1;
        }
        else
        {
            min = a;
            flagSinal = 0;
        }
    }
    else
    {
        if (strcmp (a,b) >= 0)
        {
            min = a;
            subt = b;
            flagSinal = 0;
        }
        else
        {
            min = b;
            subt = a;
            flagSinal = 1;
        }
    }
    tamMinuendo = strlen (min);
    tamSubtraendo = strlen (subt);
    char* diferenca = (char*) MALLOC (tamMinuendo+1);
    memset (diferenca, 0, tamMinuendo+1);
    for (i=tamMinuendo-1; i>=0; i--)
    {
        diferenca[i] = (min[i]-'0') - (subt[i]-'0');
        if (diferenca[i] < 0)
        {
            if (i) min[i-1]--;
            diferenca[i] += 10;
        }  
        diferenca[i] += '0';
    }
    trataZeros (&diferenca);
    return diferenca;
}

char* completaMenor (char a[], char b[], char* menor)
{
    int tamMaior = 0, tamMenor = 0, tamA = strlen(a), tamB = strlen(b);
    char __menor;
    int k = 0;
    if (tamA > tamB)
    {
        tamMaior = tamA;
        tamMenor = tamB;
        __menor = 0;
    }
    else if (tamA < tamB)
    {
        tamMaior = tamB;
        tamMenor = tamA;
        __menor = 1;
    }
    if (tamMaior + tamMenor)
    {
        char* completaZeros = (char*) MALLOC (tamMaior+1);
        completaZeros[tamMaior] = '\0';
        while (k < tamMaior-tamMenor)
        {    
            completaZeros[k] = '0';
            k++;
        }
        completaZeros[tamMaior-tamMenor] = '\0';

        if (__menor)
        {    
            strcat (completaZeros, a);
            *menor = __menor;
        }
        else
        {
            strcat (completaZeros, b);
            *menor = __menor;
        }
        return completaZeros;
    }
    else
    {
        *menor = -1;
        return NULL;
    }
}

char* multiplica (char a[],char b[])
{
    SU ta = strlen (a);
    SU tb = strlen (b);
    if ((ta == 0) || (tb == 1 && *b == '1'))
        return a;
    else if ((tb == 0) || (ta == 1 && *a == '1'))
        return b;
    char* produto = (char*) MALLOC (ta+tb+10);
    memset (produto, 0, ta+tb+10);
    int ls = 0, i, j, cursor = 0;
    for (i = tb-1; i >= 0; i--)
    {
        int carry = 0, k = ls;
        for (j = ta-1; j >= 0; j--)
        {
            int temp = (a[j] - '0') * (b[i] - '0') + carry;
            if (produto[k])
                temp += produto[k] - '0';
            produto[k++] = temp%10 + '0';
            carry = temp/10;
        }
        if (carry > 0)
            produto[k] = carry + '0';
        ls++;
        cursor=k;
    }
    produto[++cursor] = '\0';
    inverte (produto);
    return produto;
}

char* unExpo (char a[], char b[])
{ 
    char* answer = (char*) MALLOC (1000);
    char* temp = NULL;
    while(!(strcmp(b, "") || strcmp(b, "0"))) {
        swap(a, b, multiplica);
        subtrair(b, "1");
    }
    return answer;
}


/*
**  DIVISÃO POSITIVA
**  
**  N = Numerador
**  Q = Quociente
**
**  O algoritmo de divisão foi baseado na divisão do Ensino Fundamental
**
**  15467 | 58      Selecione em N o tamanho de D
**   386  |_____    -- 15
**    387  266      A subtração de 15 por 58 (D) resulta em um número positivo?
**      39           -- Não
**                  Então pegue o proximo numero de N e coloque em 15
**                  -- 154
**                  Subtraia 154 por 58 até que gerar um resultado negativo
**                  -- 154 - 58 = 96
**                  -- 96 - 58 = 38
**                  Coloque em Q quantas vezes a subtração ocorreu
**                  -- Q(0) := 2
**                  Coloque em 38 o proximo digito de N
**                  -- 386
**                  Faça a subtração novamente por D
**                  Coloque em Q quantas vezes a subtração ocorreu
**                  -- Q(1) := 6
**                  [...]
**                  O algoritmo segue até termos
**                        266 em Q
**                        39 em N (RESTO)
**
**
**  A função abaixo aplica o algoritmo acima, nele podemos visualizar que 
**  Q sempre terá len(N) - len(D) digitos 
**  
*/
char* unsigneDiv (char a[], char D[], bool MOD)
{
    const int tn = strlen (a), td = strlen (D); /* len(N) e len(D) respectivamente */
    if (td > tn)
    {
        if (MOD) return a; /* 2 % 3 == 2 */
        return NULL; /* 2 / 3 == 0 */
    }
    if (! td) return (char*) "E"; /* divisão por zero é indeterminada */
    if (*D == '1' && td == 1) return a; /* divisão por um */
    if (! strcmp (a, D)) return (char*) "1"; /* divisão de numeros iguais */
    if(!tn) return a;
    /* N, Q e o ponteiro que guarda o inicio da alocação primeira de N */
    char *N = (char*) MALLOC (tn+2);
    memset (N, 0, tn+2);
    strcpy (N, a);
    char *Q = (char*) MALLOC (tn-td+1); /* O quociente terá pelo menos tn-td digitos */
    *Q = '\0';
    char *temp = N;
    int i; /* indice de interações do laço para a divisão */
    int j = 0; /* cursor para escrita na string Q */
    int k; /* conta quantas subtrações foram feitas de N por D */ 
    int leN; /* guarda o tamanho atualizado (pela subtração) de N */
    bool fl = false; /* Marca se ocorreu ou não uma subtração de N por D */;
    for (i = 0; i < tn-td+1; i++)
    {
        if (i && strlen (Q) == tn-td) 
            break;
        k = 0;
        N[td+i%2+fl] = '\0';
        fl = false;
        while (strCmpNum (N,D))
        {
            N = subtrair (N, D);
            k++;
            fl = true;
        }
        leN = strlen (N);
        if (leN > td || !i)
            strcpy (&N[td], &a[td+i]);
        else if (fl && a[td+1])
            strcpy (&N[leN], &a[td+1]);
        if (!strCmpNum (N, D) || fl)
        {
            if (k < 10)
                Q[j++] = k + '0';
            else
            {
                int w = numDigitos (k);
                for (w--; w >= 0; w--)
                {
                    Q[j++] = k / (int) pow (10, w) + '0';
                    k %= (int) pow (10, w);
                }
                if (j > tn - td)
                {
                    Q[--j] = '\0';
                    N[td] = '\0';
                }
            }
        }
    }
    Q[j] = '\0';
    if (strcmp (N, D) < 0 && Q[j-1] == '0')
        Q[--j] = '\0';
    if (MOD) /* Se o paramento for verdadeiro, retorne o resto */
    {
        free (Q);
        trataZeros (&N);
        return N;
    }
    free (temp);
    return Q;
}

bool strCmpNum (char x[], char b[])
{
    char* a = x;
    int ta = strlen (a), tb = strlen (b);
    if (ta > tb) return 1;
    if (ta == tb)
    {
        int i;
        for (i = 0; a[i] != '\0' && b[i] != '\0'; i++)
        {
            if (a[i] > b[i]) return 1;
            else if (a[i] < b[i]) return 0;
        }
    }
    else return false;
    return true;
}

int numDigitos (int x)
{
    int i = 0;
    while (x > 0)
    {
        x /= 10;
        i++;
    }
    return i;
}

char* fatorial (char in[])
{
    char* a = (char*) MALLOC (strlen(in)+1);
    register char* fat = (char*) MALLOC (900);
    if (!a || !fat) ERRO;
    strcpy (a, in);
    int i=0,k=1, tamA = strlen (a);
    if (tamA > 3) return NULL;
    while (i<tamA)
    {
        if (! isdigit(a[i]))
        {
            a[i]= 0;
        }
        i++;
    }
    i=0;
    *fat = 1;
    LLI num = 0;
    inverte (a);
    char2int (a);
    while (i<tamA)
    {
        num += (LLI) a[i] * pow (10, i);
        i++;
    }
    if (num > 400 || num < 0) return NULL;
    for (i=2; i<=num; i++)
        k = fatorial_multiplicador (i, fat, k);
    int2char (fat, k+1);
    inverte (fat);
    return fat;
}

int fatorial_multiplicador (int a, char fat[], int limit)
{
    int count, produto, resto = 0;
    for (count = 0; count<limit; count++)
    {
        produto = fat[count] * a + resto;
        fat[count] = produto % 10;
        resto = produto / 10;
    }
    while (resto)
    {
        fat[limit] = resto % 10;
        resto = resto / 10;
        limit++;
    }
    return limit;
}

void trataZeros (char** K)
{
    char* in = *K;
    int x;
    for (x = 0; *in == '0'; x++) in++;
    if (!x) return;
    int len = strlen (in);
    char* s = (char*) MALLOC (len+1);
    s[len] = '\0';
    strcpy (s, in);
    free (in-x);
    *K = s;
}

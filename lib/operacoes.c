#ifndef INCLUSOS
    #define INCLUSOS
    #include "preproc.h"
#endif
#include <math.h>

char* soma (char op1[], char op2[]);
char* subtrair (char a[], char b[]);
char* completaMenor (char a[], char b[], char* menor);
char* multiplica (char a[], char b[]);
char* divide (char a[], char b[]);
char* fatorial (char a[]);
int fatorial_multiplicador (int a, char fat[], int limit);
int char2int (char a[]);
int int2char (char a[], int tam);
int inverte (char a[]); /* strrev (apenas para Janelas OS) */
int maior (int a, int b);
int menor (int a, int b);

/*
int main()
{
    char a[MAX];
    char b[MAX];
    char *c;
    //printf("Digite o primeiro numero: ");
    scanf("%s",a);
    //printf("Digite o segundo numero: ");
    //scanf("%s",b);
    //printf("Soma dos dois numeros: ");
    c = fatorial (a);
   // c = subtrair (a, b);
    puts (c);
    return 0;
}
*/

int inverte (char a[])
{
    int i, tam = strlen (a);
    char c;
    for (i=0; i<tam/2; i++)
    {
        c = a[i];
        a[i] = a[tam-1-i];
        a[tam-1-i] = c;
    }
    return 1;
}

int char2int (char a[])
{
    int i, tam = strlen (a);
    for (i=0; i<=tam; i++)
        a[i] = a[i] - '0';
    return 1;
}

int int2char (char a[], int tam)
{
    int i;
    for (i=0; i<tam-1; i++)
    {
        a[i] += '0';
    }
    if (! isdigit(a[i-1])) a[i-1] = '\0'; 
    a[i] = '\0';
    return 1;
}

int maior (int a, int b)
{
    return (a>=b)*a + (b>a)*b;
}

int menor (int a, int b)
{
    return (a<=b)*a + (b<a)*b;
}


char* soma (char op1[], char op2[])
{
    int ta = strlen (op1), tb = strlen (op2), ts = maior (ta, tb);
    if (! ta)
        return op2;
    else if (! tb)
        return op1;
    int i=0, resto;
    char* soma, *a, *b;
    a = (ta > tb) ? op1 : op2;
    b = (ta > tb) ? op2 : op1;
    MALLOC (soma, ts+1);
    memset (soma, 0, ts+1);
    inverte (a); inverte (b);
    char2int (a); char2int (b);
    for (i=0; i<menor(ta, tb); i++)
    {
        soma[i] += a[i] + b[i];
        if (i) soma[i] += resto;
        resto = 0;
        if (soma[i] >= 10)
        {
            soma[i] %= 10;
            resto = 1;
        }
    }
    if (ta == tb) 
        soma[i] = resto;
    else
    {
        while (i<ts)
        {
            soma[i] += a[i] + resto;
            resto = 0;
            i++;
        }
    }
    int2char (soma, ts+2);
    inverte (soma);
    while (*soma=='0') soma++;
    return soma;
}

 char* subtrair (char a[], char b[])
 {
    char *min, *subt, flagSinal, flagMenor; /* flagMenor = [ se menor == 1, então a string a é menor; se menor == 0, então string a é maior; se menor == -1, ambas têm o mesmo tamanho ] */
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

    char* diferenca = (char*) malloc (tamMinuendo+1);
    if (! diferenca) ERRO;
    for (i=0; i < tamMinuendo+1; i++)
        diferenca[i] = 0;
    inverte (min); inverte (subt);
    char2int (min); char2int (subt);
    for (i=0; i<tamMinuendo; i++)
    {
        diferenca[i] = min[i] - subt[i];
        if (diferenca[i] < 0)
        {
            min[i+1]--;
            diferenca[i] += 10;
        }  
    }
    //inverte (diferenca);
    if (tamMinuendo == 1) tamMinuendo++;
    int2char (diferenca, tamMinuendo);
    for (i=0;diferenca[i] != '\0' && diferenca[i] != '0'; i++);
    diferenca[i] = '\0';
/*   if (flagMenor)
    {
        diferenca[i] = '-';
        diferenca[i+1] = '\0';
    }
    */
    inverte (diferenca);

    if (!*diferenca) *diferenca = '0';
    return diferenca;
 }

/*
char* subtrair (char a[], char b[])
{
    char *min = NULL, *subt = NULL, flagSinal, flagMenor; /* flagMenor = [ se menor == 1, então a string a é menor; se menor == 0, então string a é maior; se menor == -1, ambas têm o mesmo tamanho ] */
/*    int i;
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

    char* diferenca;
    MALLOC (diferenca, tamMinuendo + 1);
    memset (diferenca, 0, tamMinuendo + 1);
    //inverte (min); inverte (subt);
    char2int (min); char2int (subt);
    for (i=tamMinuendo-1; i>=0; i--)
    {
        diferenca[i] = min[i] - subt[i];
        if (diferenca[i] < 0)
        {
            min[i-1]--;
            diferenca[i] += 10;
        }  
    }
    //inverte (diferenca);
    if (tamMinuendo == 1) tamMinuendo++;
    int2char (diferenca, tamMinuendo + 1);
    //while (*diferenca && *diferenca == '0') diferenca++;
    return diferenca;
}
*/
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
        char* completaZeros;
        MALLOC (completaZeros, tamMaior);
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
    char* produto;
    Int2B ta = strlen (a);
    Int2B tb = strlen (b);
    if ((ta == 0) || (tb == 1 && *b == '1'))
        return a;
    else if ((tb == 0) || (ta == 1 && *a == '1'))
        return b;
    
    MALLOC (produto, ta+tb+10);
    memset (produto, 0, ta+tb+10);
    int ls = 0, i, j, cursor = 0;
    for (i = tb-1; i >= 0; i--)
    {
        int resto = 0, k = ls;
        for (j = ta-1; j >= 0; j--)
        {
            int temp = (a[j] - '0') * (b[i] - '0') + resto;
            if (produto[k])
                temp += produto[k] - '0';
            produto[k++] = temp%10 + '0';
            resto = temp/10;
        }
        if (resto > 0)
            produto[k] = resto + '0';
        ls++;
        cursor=k;
    }
    produto[++cursor] = '\0';
    inverte (produto);
    return produto;
}

char* divide (char a[], char b[])
{
    char* quociente;
    int ta = strlen (a);
    int tb = strlen (b);
    if ((ta == 0) || (tb == 1 && *b == '1'))
        return a;
    else if ((tb == 0) || (ta == 1 && *a == '1'))
        return b;
    int len = menor (ta, tb) + 1;
    int i, j, k, ls, rs;
    MALLOC (quociente, len + 1);
    memset (quociente, 0, len + 1);
    char* tmpSubt;
    
    return quociente;
}

char* fatorial (char in[])
{
    char* a = (char*) malloc(strlen(in)+1);
    register char* fat = (char*) malloc(900);
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
    int num = 0;
    inverte (a);
    char2int (a);
    while (i<tamA)
    {
        num += (int) a[i] * pow (10, i);
        i++;
    }
    if (num > 400) return NULL;
    for (i=2; i<=num; i++)
        k = fatorial_multiplicador (i, fat, k);
    int2char (fat, k+1);
    inverte (fat);
    while (*fat == '0') fat++;
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

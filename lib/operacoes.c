#ifndef INCLUSOS
    #define INCLUSOS
    #include "preproc.h"
#endif
#include <math.h>
#define MAX 10000
char* soma (char op1[], char op2[]);
char* subtrair (char a[], char b[], char boolIGNORELFZEROS);
char* completaMenor (char a[], char b[], char* menor);
char* multiplica (char a[], char b[]);
char* divisaoPos (char a[], char D[], char boolMOD);
char* fatorial (char a[]);
int strIsDigit (char a[]);
int fatorial_multiplicador (int a, char fat[], int limit);
int char2int (char a[]);
int int2char (char a[], int tam);
int inverte (char a[]); /* strrev (apenas para Janelas OS) */
int maior (int a, int b);
int menor (int a, int b);


int main (void)
{
    char a[MAX];
    char b[MAX];
    char *c;
    //printf("Digite o primeiro numero: ");
    scanf("%s",a);
    //printf("Digite o segundo numero: ");
    scanf("%s",b);
    //printf("Soma dos dois numeros: ");
    c = divisaoPos (a, b, 0);
    puts (c);
    return 0;
}


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
    int i, resto = 0;
    char* soma, *Op2, *Op1, flag;
    Op1 = (ta >= tb) ? a : b;
    Op2 = completaMenor (a, b, &flag);
    if (! Op2)
        Op2 = (ta >= tb) ? b : a;
    MALLOC (soma, ts+2);
    memset (soma, 0, ts+2);
    *soma = '0';
    soma++;
    for (i = ts-1; i >= 0; i--)
    {
        soma[i] = (Op1[i]-'0') + (Op2[i]-'0') + resto;
        if (soma[i] >= 10)
        {
            soma[i] %= 10;
            resto = 1;
        }
        soma[i] += '0';
    }
    if (resto) *--soma += resto;
    while (*soma=='0') soma++;
    return soma;
}

 char* subtrair (char a[], char b[], char boolIGNORELFZEROS)
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
    char* diferenca;
    MALLOC (diferenca, tamMinuendo+1);
    memset (diferenca, 0, tamMinuendo+1);
    for (i=tamMinuendo-1; i>=0; i--)
    {
        diferenca[i] = (min[i]-'0') - (subt[i]-'0');
        if (diferenca[i] < 0)
        {
            min[i-1]--;
            diferenca[i] += 10;
        }  
        diferenca[i] += '0';
    }
    if (boolIGNORELFZEROS)
        while (*diferenca == '0') diferenca++;
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
    SU ta = strlen (a);
    SU tb = strlen (b);
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
/*
char* divisaoPos (char N[], char D[])
{
    int tn = strlen (N);
    int td = strlen (D);
    if ((tn == 0) || (!*N && tn == 1) || (td == 1 && *D == '1'))
        return N;
    else if (!td)
        return D;
    if (td > tn) return 0;
    long long quo = (long long) 0;
    int watch;
    int len = menor (tn, td) + 1;
    int i, resto = 0;
    char carry;
    char *R, *DwZ;
    if (td != tn)
    {
        int j, k = 0;
        MALLOC (DwZ, tn+1);
        for (j = tn - td; j > 0; j--)
            DwZ[k++] = '0';
        strcat (DwZ, D);
    }
    else DwZ = D;
    MALLOC (R, tn+1);
    *R = 0;
    strcpy (R, N);
    while (strcmp (R, DwZ))
    {
        R = subtrair (R, DwZ);
        quo++;
        if (!R) break;
    }
    if (quo < 0) return NULL;
    
    return NULL;
}
*/

char* divisaoPos (char a[], char D[], char boolMOD)
{
    int tn = strlen (a), td = strlen (D);
    int i, j = 0;
    char k;
    char *N, *R, *Q, *temp, *lnSig;
    char tmp, fl = 0;
    MALLOC (N, tn+1);
    MALLOC (lnSig, tn+1-td);
    strcpy (N, a);
    temp = N;
    N[tn] = '\0';
    MALLOC (R, 1);
    MALLOC (Q, tn-1);
    *R = 0;
    for (i = 0; i < tn-1; i++)
    {
        k = 0;
        if (td+j <= tn-1)
            tmp = N[td+j];
        N[td+j] = '\0';
        if (fl)
            strcat (lnSig, &N[td+j+1]);
        while (*N == '0') N++;
        while (strcmp (N, D) >= 0)
        {
            N = subtrair (N, D, 0);
            k++;
            fl = 1;
        }
        N[td+j] = tmp;
        N[td+j+1] = '\0';
        if (i+1 < tn-1)
            strcat (N, lnSig);
        Q[j] = k + '0';
        j++;
    }
    Q[j] = '\0';
    free (temp);
    free (lnSig);
    if (boolMOD)
    {
        free (Q);
        while (*N == '0') N++;
        return N;
    }
    free (N);
    return Q;
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

#ifndef INCLUSOS
    #define INCLUSOS
    #include "preproc.h"
#endif
#ifndef ERRO
    #define ERRO exit (3141592)
#endif
#define MAX 10000

char* soma (char a[], char b[]);
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

char* soma (char a[], char b[])
{
    int ta = strlen (a), tb = strlen (b), ts = maior (ta, tb);
    if (! ta)
        return b;
    else if (! tb)
        return a;
    int i, k, resto;
    char* soma = malloc (ts+1); /*TS+1 PORQUE A SOMA PODE GERAR MAIS 1 DIGITO A ESQUERDA (999+99 = 1098)*/
    if (!soma) ERRO;
    for (i=0; i<ts; i++)
        soma[i] = 0; /*NÃO É PRECISO TRATAR O ARRAY SOMA COMO STRING, SENÃO TERIA QUE CONVERTÊ-LO POSTERIORMENTE*/
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
    if (ta == tb) /*SE OS OPERANDOS FOREM DE IGUAL TAMANHO, COLOQUE O QUE RESTOU DA CONTA NA ULTIMA POSIÇÃO DO VETOR*/
        soma[i] = resto;
    else if (ta > tb)
    {
        while (i<ts)
        {
            soma[i] += a[i] + resto;
            resto = 0;
            i++;
        }
    }
    else
    {
        while (i<ts)
        {
            soma[i] += b[i] + resto;
            resto = 0;
            i++;
        }
    }
    int2char (soma, ts+2); /*TS + 1 PORQUE A CONTA PODE TER GERADO UM OPERADOR A MAIS*/
    inverte (soma);
    while (*soma=='0') soma++;
    return soma;
}

char* subtrair (char a[], char b[])
{
    char *min, *subt, flagSinal, flagMenor; /* flagMenor = [ se menor == 1, então a string a é menor; se menor == 0, então string a é maior; se menor == -1, ambas têm o mesmo tamanho ] */
    int i, j, k = 0;
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
        char* completaZeros = (char*) malloc (tamMaior);
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
    int ta=strlen(a)-1, tb=strlen(b)-1;
    if (ta == -1 || tb == -1) return (char*) "";
    if (! ta && *a == '1')
        return b;
    else if (! tb && *b == '1')
        return a;
    int i,j,k=0,x=0,y;
    long int r=0;
    long soma = 0;
    char *produto, *c, *temp;
    MALLOC (c, ta+tb+2);
    MALLOC (temp, ta+tb+2);
    for (i=0; i<ta+tb+2; i++)
    {
        c[i] = 0;
        temp[i] = 0;
    }
    MALLOC (produto, ta+tb+2);
    /* converte as strings a e b de digito para seu correspondente em inteiro */
    for (i=0; i<=ta; i++)
        a[i] -= '0';
    for (i=0; i<=tb; i++)
        b[i] -= '0';
    for(i=tb; i>=0; i--){
         r=0;
         for(j=ta; j>=0; j--)
         {
            temp[k++] = (b[i] * a[j] + r) % 10;
            r = (b[i] * a[j] + r) / 10;
         }
         temp[k++] = r;
         x++;
         for(y = 0; y < x; y++)
            temp[k++] = 0;
    }
    k=0;
    r=0;
    for(i=0; i<ta+tb+2; i++)
    {
        soma = 0;
        y = 0;
        for(j=1; j<=tb+1; j++)
        {
            if(i <= ta+j)
                soma += temp[y+i];
            y += j + ta + 1;
        }
        c[k++] = (soma + r) % 10;
        r = (soma + r) / 10;
    }
    c[k] = r;
    j = 0;
    for(i=k-1; i>=0; i--)
        produto[j++]=c[i] + '0';
    produto[j]='\0';
    while (*produto == '0') produto++;
    free (c);
    free (temp);
    return produto;
}

char* divide (char a[], char b[])
{

}

char* fatorial (char in[])
{
    char* a = (char*) malloc(strlen(in));
    register char* fat = (char*) malloc(500);
    if (!a || !fat) ERRO;
    strcpy (a, in);
    int i=0,k=1,j,resto=0, tamA = strlen (a);
    if (tamA > 2) return (char*) "";
    while (i<30)
    {
        if (! isdigit(a[i]))
        {
            a[i]= 0;
        }
        i++;
    }
    i=0;
    fat[0] = 1;
    int num = 0;
    inverte (a);
    char2int (a);
    while (i<tamA)
    {
        num += (int) a[i] * pow (10, i);
        i++;
    }
    if (num > 24) return (char*) "";
    for (i=2; i<=num; i++)
        k = fatorial_multiplicador (i, fat, k);
    int2char (fat, k);
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

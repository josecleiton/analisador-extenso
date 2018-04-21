#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TAM 10000

char* multiply (char a[], char b[]);
char* soma (char a[], char b[]);
char* multiplica (char a[], char b[]);
int char2int (char a[]);
int int2char (char a[], int tam);
int inverte (char a[]);
int maior (int a, int b);
int menor (int a, int b);

int main()
{
    char a[TAM];
    char b[TAM];
    char *c;
    printf("Digite o primeiro numero: ");
    scanf("%s",a);
    printf("Digite o segundo numero: ");
    scanf("%s",b);
    printf("Soma dos dois numeros: ");
    fflush (stdin);
    c = soma (a, b);
    //c = multiplica(a,b);
    puts(c);
    return 0;
}

int inverte (char a[]) //AJUSTAR ESSA FUNÇÃO, PARA QUE OS ZEROS A DIREITA FIQUEM NA ESQUERA!
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
    for (i=0; i<tam; i++)
        a[i] += 48;
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
    int ta = strlen (a), tb = strlen (b), ts = maior (ta, tb); //TAMANHO DAS STRINGS
    int i, k, resto;
    char* soma = (char*) malloc (ts+1); //TS+1 PORQUE A SOMA PODE GERAR MAIS 1 DIGITO A ESQUERDA (999+99 = 1098)
    for (i=0; i<ts; i++)
        soma[i] = 0; //NÃO É PRECISO TRATAR O ARRAY SOMA COMO STRING, PORQUE SENÃO TERIA QUE CONVERTÊ-LO POSTERIORMENTE
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
            resto = 1; //RESTO VAI SER OU UM OU ZERO!
        }
    }
    //printf("\ni = %d\n", i);
    if (ta == tb) //SE OS OPERANDOS FOREM DE IGUAL TAMANHO, COLOQUE O QUE RESTOU DA CONTA NA ULTIMA POSIÇÃO DO VETOR
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
    //for (k=ts; k>=0; k--)
    //    printf("%d\n", soma[k]);
    //printf("%d\n", soma[i]);
    int2char (soma, ts+1); //TS + 1 PORQUE A CONTA PODE TER GERADO UM OPERADOR A MAIS
    inverte (soma);
    return soma;
}

char* multiplica (char a[], char b[])
{

}

char* multiply (char a[],char b[])
{
    int ta,tb;
    int i,j,k=0,x=0,y;
    long int r=0;
    long soma = 0;
    ta=strlen(a)-1;
    tb=strlen(b)-1;
    char* produto = (char*) malloc (ta+tb+2);
    char c[ta+tb+2];
    char temp[ta+tb+2];
    char2int (a);
    char2int (b);
    for(i=tb;i>=0;i--){
         r=0;
         for(j=ta;j>=0;j--)
         {
            temp[k++] = (b[i]*a[j] + r)%10;
            r = (b[i]*a[j]+r)/10;
         }
         temp[k++] = r;
         x++;
         for(y = 0;y<x;y++)
            temp[k++] = 0;
    }
    k=0;
    r=0;
    for(i=0;i<ta+tb+2;i++)
    {
        soma = 0;
        y = 0;
        for(j=1;j<=tb+1;j++)
        {
            if(i <= ta+j)
            {
                soma = soma + temp[y+i];
            }
            y += j + ta + 1;
        }
        c[k++] = (soma+r) %10;
        r = (soma+r)/10;
    }
    c[k] = r;
    j = 0;
    for(i=k-1;i>=0;i--)
    {
        produto[j++]=c[i] + 48;
    }
    produto[j]='\0';
    return produto;
}

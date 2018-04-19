#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define TAM 10000

char* multiplica (char a[], char b[]);
int main()
{
    char a[TAM];
    char b[TAM];
    char *c;
    printf("Digite o primeiro numero: ");
    scanf("%s",a);
    printf("Digite o segundo numero: ");
    scanf("%s",b);
    printf("Produto dos dois numeros: ");
    c = multiplica(a,b);
    printf("%s\n",c);
    return 0;
}

char* multiplica (char a[],char b[])
{
    static char produto[TAM];
    char c[TAM];
    char temp[TAM];
    int ta,tb;
    int i,j,k=0,x=0,y;
    long int r=0;
    long soma = 0;
    ta=strlen(a)-1;
    tb=strlen(b)-1;
        for(i=0;i<=ta;i++)
            a[i] = a[i] - 48;

        for(i=0;i<=tb;i++)
            b[i] = b[i] - 48;

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
        soma =0;
        y=0;
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
    j=0;
    for(i=k-1;i>=0;i--)
    {
        produto[j++]=c[i] + 48;
    }
    produto[j]='\0';
    return produto;
}

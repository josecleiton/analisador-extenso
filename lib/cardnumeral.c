/*
*   ENTRADA NUMERO POR EXTENSO
*   SAIDA NUMERO EM NUMERAL
*   EXEMPLO: QUINHENTOS E TRINTA E SETE  -> 537
*
*/

#ifndef INCLUSOS
    #define INCLUSOS
    #include "preproc.h"
#endif
#ifndef ERRO 
    #define ERRO exit (3141592)
#endif
#define TAM 26

typedef struct node Node;
struct node
{
    char info[30];
    Node *prox;
};
char* analiSemantica (char* expressao, Ordem* ref);
void stackPush (Node** topo, char s[]);
char* convNumeral (char* extenso, Ordem* ref);

char* analiSemantica (char* expressao, Ordem* ref)
{
    expressao = convNumeral (expressao, ref);
    return expressao;
}

char* convNumeral (char* extenso, Ordem* ref)
{
    int cursor = 0, i = 0, k = 0;
    char *aux = extenso, *resultado, *classe;
    MALLOC (resultado, 2);
    MALLOC (classe, 2);
    resultado[0] = '0';
    resultado[1] = '\0';
    classe[0] = '0';
    classe[1] = '\0';
    while (*aux != '\0')
    {
        k=0;
        while (aux[k] != '\0' && aux[k] != '-')
            k++;
        aux[k] = '\0';
        i=0;
        while (i<37)
        {
            if (*aux == 'e') break;
            if (strstr (ref[i].nome, aux))
            {
                classe = soma (ref[i].valor, classe);
                break;
            }
            i++;
        }
        while (i>=37)
        {
           if (strstr (ref[i].nome, aux))
           {
               resultado = soma (multiplica (ref[i].valor, classe), resultado);
               classe[0] = '0';
               classe[1] = '\0';
               break;
           }
           i++;
        }
        aux += k+1;
    }
    return resultado;
}

void stackPush (Node** topo, char s[])
{
    Node *no, *aux = *topo;
    MALLOC (no, 30);
    strcpy (no -> info, s);
    no -> prox = *topo;
    *topo = no;
}


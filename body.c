#include "lib/preproc.h"
#define TAM 22
#define ARQ_ORDENS "lib/ordens.txt"
typedef struct ordem Ordem;
struct ordem
{
    char* nome;
    char* valor;
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
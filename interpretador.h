#ifndef _INTERPRETADOR_H_
#define _INTERPRETADOR_H_
/*
**  Inclusão do header com as bibliotecas e macros utilizados pelos arquivos:
**  main.c e operacoes.c
*/

#include "preproc.h"

/*
**  DICIONÁRIO
**  <palavra>=<valor>
**  
**  palavra é o número por extenso aceito
**  valor é o número cardinal em linguagem matemática
**  Exemplo: trezentos=300
*/
#define ARQ_DICT "lib/dicionario.cfg"

/*
**  ERROS
**  
**  Um tipo de erro por linha
*/
#define ARQ_ERROS "lib/erros.cfg"

/*
**  ENTRADA
** 
**  expressões a serem analisadas pelo programa
*/
#define ARQ_ENTRADA "lib/expressoes.txt"

/*
**  SAIDA
**  resultado das expressões analisadas
*/
#define ARQ_SAIDA "resultados.txt"

/*
**  LOG
**  
**  data da ocorrencia de algum erro na expressão, seguido da descrição do mesmo
*/
#define ARQ_LOG "logs.txt"

/*
**  TAM_DICT
**  Número de linhas do ARQ_DICT
*/
#define TAM_DICT 58

/*
**  STR_DEL
**  String com os caracteres dos delimitadores
*/

#define STR_DEL "+/%-*!e()^"

/*
**  INDEL
**  linha do ARQ_DIC na qual começam os delimitadores
*/
#define INDEL 48 

/*
**  NUM_ERROS
**  número de linhas do ARQ_ERROS
*/
#define NUM_ERROS 13 

/*
**  MAX_GEN
**  tamanhos genéricos de string
*/
#define MAX_GEN 1024

/*
**  MAXWLEN
**  tamanho maximo das palavras no ARQ_DICT (antes e depois do '=')
*/
#define MAXWLEN 35

/*
**  CLRBUF
**  limpeza do buffer de entrada
*/
#define CLRBUF scanf ("%*c")

/*
*   ABAIXO SEGUEM AS DECLARAÇÕES, RESPECTIVAMENTE:
*   Struct para guardar temporariamente uma linha do ARQ_DICT
*   Lista encadeada que carregara todo o número a ser analisado semanticamente
*/
typedef struct ordem
{
    char nome[MAXWLEN];
    char valor[MAXWLEN];
} Ordem;

typedef struct filanum 
{
    short classe;
    Ordem *info;
    struct filanum *ant, *prox;
} FilaNum;

typedef struct index{
    SU* index;
    int tam;
} Index;

/*
**  GATILHO DE PARTIDA A PARTIR DE UM ARQUIVO
*/
int fileParsingInit (void);

/*
**  MOSTRA O ARQUIVO DE RESULTADO
*/
void printRes(void);

/*
** RETORNA O TAMANHO DA MAIOR STRING NO ARQUIVO
*/
size_t maiorString (FILE* stream);

/*
**  GATILHO DE PARTIDA 
*/
char* expParsingStart (void);

/*
**  ROTINA QUE SOMA OU SUBTRAI TERMOS
*/
void expResTermo (char* resposta);

/*
**  ROTINA QUE DIVIDE OU MULTIPLICA FATORES 
*/
void expResFator (char* resposta);

/*
**  ROTINA QUE RESOLVE O FATORIAL DE UM FATOR
*/
void expResFatorial (char* resposta);

/*
**  ROTINA QUE RESOLVE UMA EXPRESSÃO DENTRO DE PARENTESES
*/
void expResParenteses (char* resposta);

/* EM CONSTRUÇÃO void expAvalSinal (char* resposta);  AVALIA + OU - UNÁRIO */

/*
**  DEVOLVE O VALOR NUMERICO DAS EXPRESSÕES POR EXTENSO
*/
void atomo (char* resposta); 

/*
**  VERIFICA A EXISTENCIA DA PALAVRA NA EXPRESSÃO COM ALGUMA DO DICIONÁRIO, SE EXISTIR, GUARDA EM TOKEN O PRIMEIRO CARACTER DA MESMA
*/
void pega_token (void);

/*
**  PULA A PALAVRA EM ANALISE
*/
void ajustaEXP (void);

/*
**  RETORNA 1 SE O PROX TOKEN FOR UM DELIMITADOR
*/
bool verificaProxToken (void); 

/*
**  EM ORDENS COMPOSTAS, AVALIA TANTO A FORMA PLURAL QUANTO SINGULAR E ENFILA A FORMA INSERIDA
*/
bool resPlural (int i, char *s);

/*
**  AJUSTA DELIMITADORES COMPOSTOS COLOCANDO HÍFEN ENTRE AS PALAVRAS
**  Entrada: dividido por
**  Saida: dividido-por
**
**  DELIMITADORES COMPOSTOS:
**  dividido-por
**  fatorial-de
**  abre-parentese
**  fecha-parentese
*/
void ajustaDelim (int* k, char* temp);

/*
**  TODOS OS POSSÍVEIS ERROS (CHECAR ARQ_ERROS)
*/
void erroSS (int tipoErro); 

Index criaIndices (FILE* in, int limite); 

/*
**  DEVOLVE EM out UM VETOR COM AS POSIÇÕES DE del NO ARQUIVO in
*/
SU* _criaIndices (FILE* in, int size, int del); 

/*
**  ANALISA O SIGNIFICADO DA EXPRESSÃO
*/
bool analiSemantica (void); 

/*
**  ANALISA O SIGNIFICADO DA CENTENA/DEZENA/UNIDADE NA EXPRESSÃO
*/
bool semUnidade (FilaNum** inicio); 

/*
**  ANALISA O SIGNIFICADO DO PLURAL DE ORDENS (>= MIL) NA EXPRESSÃO
*/
void pluralOrdem (FilaNum* inicio); 

/*
**  RETORNA A ORDEM DO NUMERO APONTADO POR INICIO
*/
SU pegaOrdem (FilaNum* inicio); 

/*
**  CONVERTE DE EXTENSO PARA UMA STRING DE DIGITOS
**  
**  
*/
char* toNum (void);

/*
**  CONVERTE DE UMA STRING DE DIGITOS PARA EXTENSO
*/
void toName (char** resposta); 

/*
**  CONVERTE A C/D/U PARA EXTENSO
*/
int toNameMenOrd (char** str, char* resultado, SU* size, SU* flagPlural); 

/*
**  INSERÇÃO COMO FILA NUMA LISTA ENCADEADA
*/
void filaInsere (SU i, char* nome, char* valor); 

/*
**  DEVOLVE O NÓ DO PROXIMO NUMERO NA FILA APONTADA POR INICIO
*/
FilaNum* pegaProxNum (FilaNum* inicio); 

/*
**  DEVOLVE A PROXIMA CLASSE APONTADA POR INICIO
*/
SU pegaProxClasse (FilaNum* inicio); 

/*
**  LIBERA A LISTA
*/
void filaLibera (void); 

/*
**  CONTA OS NÓS DA LISTA
*/
int filaCount (void); 

/*
**  CONTA QUANTAS LINHAS TEM O ARQUIVO IN
*/
int fstrcount (FILE* in);

/*
**  MÉTODO PORTÁVEL DE LIMPAR A TELA
*/
void clearScreen (void); 

/*
**  CONVERTE POSSÍVEIS CARACTERES MAÍUSCULOS DE EXP PARA MINÚSCULOS
*/
void strToLower (void);

/*
**  OPENFILE
**  função para abertura de arquivos
*/
FILE* OPENFILE(const char file_name[], const char type[]);

extern char *EXP; /* Ponteiro para expNum */
extern char *_TEXP; /* guarda a expressão sem modificações, para a possível exibição de erros */
extern char expNum[MAX_GEN]; /* Expressão que será analisada */

#endif

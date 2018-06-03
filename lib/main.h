#ifndef INCLUSOS
    #define INCLUSOS
    #include "preproc.h" /* Header com as bibliotecas desse arquivo e do auxiliar "operacoes.c" */
#endif

/*
*   ABAIXO SEGUEM AS DECLARAÇÕES, RESPECTIVAMENTE:
*   Struct para guardar temporariamente uma linha do ARQ_DICT
*   Lista encadeada que carregara todo o número a ser analisado semanticamente
*/
typedef struct ordem
{
    char* nome;
    char* valor;
} Ordem;
typedef struct filanum 
{
    short classe;
    Ordem *info;
    struct filanum *ant, *prox;
} FilaNum;

/*
**  GATILHO DE PARTIDA A PARTIR DE UM ARQUIVO
*/
int fileParsingInit (void);

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
int verificaProxToken (void); 

/*
**  EM ORDENS COMPOSTAS, AVALIA TANTO A FORMA PLURAL QUANTO SINGULAR E ENFILA A FORMA INSERIDA
*/
int resPlural (int i, char** s);

/*
**  AJUSTA DELIMITADORES COMPOSTOS COLOCANDO HÍFEN ENTRE AS PALAVRAS
**  Entrada: dividido por
**  Saida: dividido-por
*/
void ajustaDelim (int* k, char* temp);

/*
**  TODOS OS POSSÍVEIS ERROS (CHECAR lib/erros.txt)
*/
void erroSS (int tipoErro); 

/*
**  DEVOLVE EM out UM VETOR COM AS POSIÇÕES DE del NO ARQUIVO in
*/
void criaIndices (FILE* in, SU** out, int size, int del); 

/*
**  ANALISA O SIGNIFICADO DA EXPRESSÃO
*/
int analiSemantica (void); 

/*
**  ANALISA O SIGNIFICADO DA CENTENA/DEZENA/UNIDADE NA EXPRESSÃO
*/
int semUnidade (FilaNum** inicio); 

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

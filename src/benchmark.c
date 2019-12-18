/*
**  Universidade Estadual do Estado da Bahia - UNEB
**  Por: José Cleiton Borges Correia
**  Prof: Jorge Sampaio Farias
**  Disciplina: Linguagem de Programação 1 - 2018.1
**  #################################################
**  | AVALIADOR DE EXPRESSÕES NUMERICAS POR EXTENSO |
**  | Entrada: expressão numérica por extenso       |
**  | Saída: expressão resolvida por extenso        |
**  #################################################
*/
#include <time.h>

#include "interpretador.h"

int main(void) {
   EXP = expNum;
   char expBucket[BUFSIZ] = {'\0'};
   float startTime = (float)clock() / CLOCKS_PER_SEC;
   printf(
       "\tForam analisadas e resolvidas %d expressoes.\n\tOs resultado "
       "podem ser encontrados em %s\n",
       fileParsingInit(expBucket), ARQ_SAIDA);
   float endTime = (float)clock() / CLOCKS_PER_SEC;
   printf("Passou %f tempo\n", endTime - startTime);
   return 0;
}

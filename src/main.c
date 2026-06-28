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
#include <string.h>
#include "extenso/cli.h"

int main (int argc, char **argv)
{
    /* Modo batch não-interativo (usado pelo teste de regressão):
    ** lê ARQ_ENTRADA, escreve ARQ_SAIDA e sai, sem o menu. */
    if (argc > 1 && strcmp (argv[1], "--batch") == 0)
    {
        fileParsingInit ();
        return 0;
    }
    return interpretador ();
}

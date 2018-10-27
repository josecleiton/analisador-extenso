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
#include "interpretador.h"

int main (void)
{
    EXP = expNum;
    char* resultado, op;
    puts ("\n\t\tANALISADOR DE EXPRESSOES NUMERICAS POR EXTENSO\n");
    CLRBUF;
    while(true)
    {
        clearScreen ();
        puts ("Selecione a entrada:\n a= Arquivo\n t= Teclado\n e= Sair\n\nopcao = ");
        scanf ("%c%*c", &op);
        switch (op)
        {
            case 'a':
                clearScreen ();
                printf ("\tForam analisadas e resolvidas %d expressoes.\n\tOs resultados podem ser encontrados em %s\n", fileParsingInit (), ARQ_SAIDA);
                printRes();
                CLRBUF;
                break;
            case 't':
                clearScreen ();
                puts ("Digite uma expressao numerica: ");
                scanf ("%[^\n]%*c", EXP);
                resultado = expParsingStart ();
                printf ("\n\tResultado: %s\n", resultado);
                free (_TEXP);
                CLRBUF;
                break;
            case 'e': return 0;
            default: 
                CLRBUF;
                puts ("Opcao invalida.\n");
        }
    }
}

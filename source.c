/*
    ESTRUTURA DO ANALISADOR:
    identificador | comando de atribuição | expressão em português
    var := quinhentos e sessenta e nove mais cinco
    
    conversão para
    identificador | comando de atribuição | expressão convertida para expressão matemática
    var := 569+5
*/
/*

POSSÍVEIS TOKENS

enum tokens
{
    tk_abrpar '('
    tk_fecpar ')'
    tk_uni {0 - 19}
    tk_dez {10 - 90}
    tk_cen {100}
    tk_mil {1000}
    tk_mlh 
    tk_blh
    tk_tlh
    tk_id
    tk_add e = '+'
    tk_prd vezes/por = '*' //ligação entre unidades e centenas, milhares e etc
    tk_sub menos = '-'
    tk_div dividido por = '/'
    tk_mod mod/mod de = '%'
    tk_erro

    //RESTRIÇÃO DE SINTAXE, SE APARECER UMA ORDEM DE GRANDEZA X, É UM ERRO APARECER UMA ORDEM MAIOR QUE X. EX = 1 BILHÃO E DUZENTOS TRILHÕES 

}

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lib/body.h"
#define FOR(i,a) for(int i=0; i<int(a); i++)

int main (void)
{
    StringMatrix dic = cria_dic();
    return 0;
}
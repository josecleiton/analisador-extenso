#include <stdio.h>
#include <stdlib.h>
#define ERRO 3.141592


/*
*   FUNÇÃO QUE DEVOLVE O "DICIONÁRIO" PARA
*   A ANÁLISE DOS NUMEROS PRESENTES DE FORMA 
*   EXTENSA NAS EXPRESSÕES
*/
char*** cria_dic (void)
{
    int i;
    char*** ref = (char***) malloc (sizeof(char**)*5); // APONTADOR DE APONTADORES PARA OS VETORES DE STRINGS ABAIXO
    char** unit = (char**) malloc (sizeof(char*)*19); // UNIDADES
    char** dez = (char**) malloc (sizeof(char*)*8); // DEZENAS
    char** cent = (char**) malloc (sizeof(char*)*9); // CENTENAS
    char** mil = (char**) malloc (sizeof(char**)*4); // MIL | MILHÃO | TRILHÃO
    char** other = (char**) malloc (sizeof(char**)*4); // CEM | MILHÕES | BILHÕES | TRILHÕES
    if (!ref || !unit || !dez || !cent || !mil) exit (ERRO);

    for (i=0; i<19; i++)
    {
        unit[i] = (char*) malloc (10);
        if (!unit[i]) exit (ERRO);
    }
    unit[0] = "um";
    unit[1] = "dois";
    unit[2] = "tres";
    unit[3] = "quatro";
    unit[4] = "cinco";
    unit[5] = "seis";
    unit[6] = "sete";
    unit[7] = "oito";
    unit[8] = "nove";
    unit[9] = "dez";
    unit[10] = "onze";
    unit[11] = "doze";
    unit[12] = "treze";
    unit[13] = "catorze";
    unit[14] = "quinze";
    unit[15] = "dezesseis";
    unit[16] = "dezessete";
    unit[17] = "dezoito";
    unit[18] = "dezenove";
    
    for (i=0; i<8; i++)
    {
        dez[i] = (char*) malloc (10);
        if (!dez[i]) exit (ERRO);
    }
    dez[0] = "vinte";
    dez[1] = "trinta";
    dez[2] = "quarenta";
    dez[3] = "cinquenta";
    dez[4] = "sessenta";
    dez[5] = "setenta";
    dez[6] = "oitenta";
    dez[7] = "noventa";
    
    for (i=0; i<9; i++)
    {
        cent[i] = (char*) malloc (13);
        if (!cent[i]) exit (ERRO);
    }
    cent[0] = "cento";
    cent[1] = "duzentos";
    cent[2] = "trezentos";
    cent[3] = "quatrocentos";
    cent[4] = "quinhentos";
    cent[5] = "seiscentos";
    cent[6] = "setecentos";
    cent[7] = "oitocentos";
    cent[8] = "novecentos";

    for (i=0; i<5; i++)
    {
        mil[i] = (char*) malloc (8);
        if (!mil[i]) exit (ERRO);
    }
    mil[0] = "mil";
    mil[1] = "milhao";
    mil[2] = "bilhao";
    mil[3] = "trilhao";

    for (i=0; i<5; i++)
    {
        mil[i] = (char*) malloc (9);
        if (!mil[i]) exit (ERRO);
    }
    other[0] = "cem";
    other[1] = "milhoes";
    other[2] = "bilhoes";
    other[3] = "trilhoes";

    ref[0] = unit;
    ref[1] = dez;
    ref[2] = cent;
    ref[3] = mil;
    ref[4] = other;

    return ref;
}
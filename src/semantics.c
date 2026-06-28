#include "extenso/semantics.h"
#include "extenso/state.h"
#include "extenso/tokens.h"
#include "extenso/num_list.h"
#include "extenso/errors.h"

bool analiSemantica (void)
{
    ListaNum* listSem = list;
    if (! listSem) erroSS (3);
    if (listaCount() > (DECILHAO-NOVECENTOS)*4-1) erroSS (7); /* LIMITE DE DECILHÕES */
    uint16_t ord[2], i = 0;
    while (listSem)
    {
        pluralOrdem(listSem);
        semUnidade (&listSem);
        ord[i%2] = pegaOrdem (listSem);
        if (i++%2 && ord[0] <= ord[1]) erroSS (2);
        if (listSem) listSem = listSem -> prox;
    }
    return true;
}

void pluralOrdem (ListaNum* inicio)
{
    ListaNum* aux = inicio;
    while (aux && (aux -> classe < MILHAO || aux -> classe == CONJUCAO)) aux = aux -> prox;
    if (aux && strstr (aux -> info -> nome, (const char*) "oes"))
    {
        if (inicio -> classe != UM || inicio -> classe == CONJUCAO) return;
    }
    else if (!aux || inicio -> classe == UM || inicio -> classe == CONJUCAO) return;
    erroSS (12);
}

bool semUnidade (ListaNum** inicio)
{
    ListaNum *lista = *inicio;
    bool flag = false;
    while (lista && (lista -> classe < MIL || lista -> classe == CONJUCAO))
    {
        if (lista -> classe < VINTE && lista -> classe != DEZ)
        {
            if (lista -> prox)
            {
                if (lista -> prox -> classe == CONJUCAO) erroSS (11);
                if (lista -> prox -> classe < MIL) erroSS (2);
            }
        }
        else if (lista -> classe == DEZ || (lista -> classe >= VINTE && lista -> classe <= NOVENTA))
        {
            if (lista -> prox && (lista -> prox -> classe < MIL || lista -> prox -> classe == CONJUCAO))
            {
                if (lista -> prox -> classe != CONJUCAO) erroSS (9);
                else if (lista -> prox -> prox == NULL) erroSS (10);
                else if (lista -> prox -> prox -> classe > NOVE) erroSS (2);
            }
        }
        else if (lista -> classe >= CEM && lista -> classe <= NOVECENTOS)
        {
            if (!strcmp (lista -> info -> nome, (const char*) "cem") && (lista -> prox && lista -> prox -> classe == CONJUCAO)) erroSS (12);
            if (lista -> prox && (lista -> prox -> classe < MIL || lista -> prox -> classe == CONJUCAO))
            {
                if (lista -> prox -> classe != CONJUCAO) erroSS (9);
                else if (lista -> prox -> prox == NULL) erroSS (10);
                else if (lista -> prox -> prox -> classe > NOVENTA) erroSS (2);
            }
        }
        lista = lista -> prox;
        flag = true;
    }
    *inicio = lista;
    if (! flag) erroSS (5);
    return flag;
}

uint16_t pegaOrdem (ListaNum* inicio)
{
    ListaNum* aux = inicio;
    while (aux && (aux -> classe < MIL || aux -> classe == CONJUCAO)) aux = aux -> prox;
    if (! aux) return NOVECENTOS;
    return aux -> classe;
}

uint16_t pegaProxClasse (ListaNum* inicio)
{
    uint16_t classe = 0;
    if (! inicio) return classe;
    while (inicio -> classe >= MIL) inicio = inicio -> prox;
    classe = inicio -> classe;
    return classe;
}

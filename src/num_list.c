#include "extenso/num_list.h"
#include "extenso/state.h"
#include "extenso/util.h"
#include "extenso/tokens.h"

void listaInsere (uint16_t i, char *nome, char *valor)
{
    ListaNum *no = (ListaNum*) alloc (1, sizeof (ListaNum));
    ListaNum *aux = list;
    no -> info = (Ordem*) alloc (1, sizeof (Ordem));
    strcpy (no->info->nome, nome);
    strcpy (no->info->valor, valor);
    no -> classe = i;
    no -> prox = NULL;
    if (! list)
    {
        list = no;
        no -> ant = NULL;
        return;
    }
    while (aux && aux->prox)
        aux = aux -> prox;
    no -> ant = pegaProxNum (list);
    aux -> prox = no;
}

ListaNum *pegaProxNum (ListaNum *inicio)
{
    ListaNum* aux = NULL;
    while (inicio)
    {
        if (inicio -> classe < MIL) aux = inicio;
        inicio = inicio -> prox;
    }
    return aux;
}

void listaLibera (void)
{
    ListaNum *aux = list, *zombie = NULL;
    while (aux)
    {
        zombie = aux;
        aux = aux -> prox;
        if (zombie->info)
            free (zombie->info);
        free (zombie);
    }
    list = NULL;
}

int listaCount (void)
{
    ListaNum* aux;
    int n;
    for (n = 0, aux = list; aux; aux = aux -> prox, n++);
    return n;
}

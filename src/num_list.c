#include "extenso/num_list.h"
#include "extenso/util.h"
#include "extenso/tokens.h"

void listaInsere (Context *ctx, uint16_t i, char *nome, char *valor)
{
    ListaNum *no = (ListaNum*) alloc (1, sizeof (ListaNum));
    ListaNum *aux = ctx->list;
    no -> info = (Ordem*) alloc (1, sizeof (Ordem));
    strcpy (no->info->nome, nome);
    strcpy (no->info->valor, valor);
    no -> classe = i;
    no -> prox = NULL;
    if (! ctx->list)
    {
        ctx->list = no;
        no -> ant = NULL;
        return;
    }
    while (aux && aux->prox)
        aux = aux -> prox;
    no -> ant = pegaProxNum (ctx->list);
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

void listaLibera (Context *ctx)
{
    ListaNum *aux = ctx->list, *zombie = NULL;
    while (aux)
    {
        zombie = aux;
        aux = aux -> prox;
        if (zombie->info)
            free (zombie->info);
        free (zombie);
    }
    ctx->list = NULL;
}

int listaCount (Context *ctx)
{
    ListaNum* aux;
    int n;
    for (n = 0, aux = ctx->list; aux; aux = aux -> prox, n++);
    return n;
}

#include "extenso/num_list.h"
#include "extenso/tokens.h"
#include "extenso/util.h"

void
listAppend (Context *ctx, uint16_t i, char *name, char *value)
{
    NumList *no = (NumList *)alloc (1, sizeof (NumList));
    NumList *aux = ctx->list;
    no->info = (Term *)alloc (1, sizeof (Term));
    strcpy (no->info->name, name);
    strcpy (no->info->value, value);
    no->cls = i;
    no->next = NULL;
    if (!ctx->list)
        {
            ctx->list = no;
            no->prev = NULL;
            return;
        }
    while (aux && aux->next)
        aux = aux->next;
    no->prev = lastNumberNode (ctx->list);
    aux->next = no;
}

NumList *
lastNumberNode (NumList *inicio)
{
    NumList *aux = NULL;
    while (inicio)
        {
            if (inicio->cls < MIL)
                aux = inicio;
            inicio = inicio->next;
        }
    return aux;
}

void
listFree (Context *ctx)
{
    NumList *aux = ctx->list, *zombie = NULL;
    while (aux)
        {
            zombie = aux;
            aux = aux->next;
            if (zombie->info)
                free (zombie->info);
            free (zombie);
        }
    ctx->list = NULL;
}

int
listCount (Context *ctx)
{
    NumList *aux;
    int n;
    for (n = 0, aux = ctx->list; aux; aux = aux->next, n++)
        ;
    return n;
}

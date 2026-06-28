#include "extenso/semantics.h"
#include "extenso/errors.h"
#include "extenso/num_list.h"
#include "extenso/tokens.h"

bool
checkSemantics (Context *ctx)
{
    NumList *listSem = ctx->list;
    if (!listSem)
        reportError (ctx, 3);
    if (listCount (ctx) > (DECILHAO - NOVECENTOS) * 4 - 1)
        reportError (ctx, 7); /* DECILLION LIMIT */
    uint16_t ord[2], i = 0;
    while (listSem)
        {
            checkPlural (ctx, listSem);
            checkUnit (ctx, &listSem);
            ord[i % 2] = orderOf (listSem);
            if (i++ % 2 && ord[0] <= ord[1])
                reportError (ctx, 2);
            if (listSem)
                listSem = listSem->next;
        }
    return true;
}

void
checkPlural (Context *ctx, NumList *inicio)
{
    NumList *aux = inicio;
    while (aux && (aux->cls < MILHAO || aux->cls == CONJUCAO))
        aux = aux->next;
    if (aux && strstr (aux->info->name, (const char *)"oes"))
        {
            if (inicio->cls != UM || inicio->cls == CONJUCAO)
                return;
        }
    else if (!aux || inicio->cls == UM || inicio->cls == CONJUCAO)
        return;
    reportError (ctx, 12);
}

bool
checkUnit (Context *ctx, NumList **inicio)
{
    NumList *lista = *inicio;
    bool flag = false;
    while (lista && (lista->cls < MIL || lista->cls == CONJUCAO))
        {
            if (lista->cls < VINTE && lista->cls != DEZ)
                {
                    if (lista->next)
                        {
                            if (lista->next->cls == CONJUCAO)
                                reportError (ctx, 11);
                            if (lista->next->cls < MIL)
                                reportError (ctx, 2);
                        }
                }
            else if (lista->cls == DEZ || (lista->cls >= VINTE && lista->cls <= NOVENTA))
                {
                    if (lista->next && (lista->next->cls < MIL || lista->next->cls == CONJUCAO))
                        {
                            if (lista->next->cls != CONJUCAO)
                                reportError (ctx, 9);
                            else if (lista->next->next == NULL)
                                reportError (ctx, 10);
                            else if (lista->next->next->cls > NOVE)
                                reportError (ctx, 2);
                        }
                }
            else if (lista->cls >= CEM && lista->cls <= NOVECENTOS)
                {
                    if (!strcmp (lista->info->name, (const char *)"cem") && (lista->next && lista->next->cls == CONJUCAO))
                        reportError (ctx, 12);
                    if (lista->next && (lista->next->cls < MIL || lista->next->cls == CONJUCAO))
                        {
                            if (lista->next->cls != CONJUCAO)
                                reportError (ctx, 9);
                            else if (lista->next->next == NULL)
                                reportError (ctx, 10);
                            else if (lista->next->next->cls > NOVENTA)
                                reportError (ctx, 2);
                        }
                }
            lista = lista->next;
            flag = true;
        }
    *inicio = lista;
    if (!flag)
        reportError (ctx, 5);
    return flag;
}

uint16_t
orderOf (NumList *inicio)
{
    NumList *aux = inicio;
    while (aux && (aux->cls < MIL || aux->cls == CONJUCAO))
        aux = aux->next;
    if (!aux)
        return NOVECENTOS;
    return aux->cls;
}

uint16_t
nextClass (NumList *inicio)
{
    uint16_t cls = 0;
    if (!inicio)
        return cls;
    while (inicio->cls >= MIL)
        inicio = inicio->next;
    cls = inicio->cls;
    return cls;
}

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
    NumList *magnitude = inicio;
    while (magnitude && (magnitude->cls < MILHAO || magnitude->cls == CONJUCAO))
        magnitude = magnitude->next;
    if (!magnitude || inicio->cls == CONJUCAO)
        return;
    /* "um" must pair with the singular form; anything else with the plural
    ** ("um milhao" / "dois milhoes"). An error is exactly the mismatch. */
    bool leadingOne = (inicio->cls == UM);
    bool pluralForm = strstr (magnitude->info->name, (const char *)"oes") != NULL;
    if (leadingOne == pluralForm)
        reportError (ctx, 12);
}

/* After a unit (< 20) the next token may not be a conjunction nor another
** sub-magnitude part. */
static void
checkAfterUnit (Context *ctx, NumList *node)
{
    if (!node->next)
        return;
    if (node->next->cls == CONJUCAO)
        reportError (ctx, 11);
    if (node->next->cls < MIL)
        reportError (ctx, 2);
}

/* After a ten/hundred, a smaller part must come through the conjunction 'e'
** followed by a value no greater than `maxAfter`. */
static void
checkConjunctionAfter (Context *ctx, NumList *node, short maxAfter)
{
    if (!node->next || (node->next->cls >= MIL && node->next->cls != CONJUCAO))
        return;
    if (node->next->cls != CONJUCAO)
        reportError (ctx, 9);
    else if (node->next->next == NULL)
        reportError (ctx, 10);
    else if (node->next->next->cls > maxAfter)
        reportError (ctx, 2);
}

bool
checkUnit (Context *ctx, NumList **inicio)
{
    NumList *node = *inicio;
    bool sawAny = false;
    while (node && (node->cls < MIL || node->cls == CONJUCAO))
        {
            if (node->cls < VINTE && node->cls != DEZ)
                checkAfterUnit (ctx, node);
            else if (node->cls == DEZ || (node->cls >= VINTE && node->cls <= NOVENTA))
                checkConjunctionAfter (ctx, node, NOVE);
            else if (node->cls >= CEM && node->cls <= NOVECENTOS)
                {
                    if (!strcmp (node->info->name, (const char *)"cem") && node->next && node->next->cls == CONJUCAO)
                        reportError (ctx, 12);
                    checkConjunctionAfter (ctx, node, NOVENTA);
                }
            node = node->next;
            sawAny = true;
        }
    *inicio = node;
    if (!sawAny)
        reportError (ctx, 5);
    return sawAny;
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

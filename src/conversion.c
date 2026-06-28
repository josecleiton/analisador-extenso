#include "extenso/conversion.h"
#include "extenso/bignum.h"
#include "extenso/num_list.h"
#include "extenso/semantics.h"
#include "extenso/tokens.h"
#include "extenso/util.h"

/*
**  Normalizes output spacing in-place: collapses runs of spaces into a single
**  one and removes leading and trailing spaces. Guarantees clean output
**  regardless of the ad-hoc separators assembled in toWords/toWordsTriplet.
*/
static void
normalizeSpaces (char *s)
{
    char *r = s, *w = s;
    int pendente = 0;
    while (*r == ' ')
        r++; /* skip leading spaces */
    while (*r)
        {
            if (*r == ' ')
                {
                    pendente = 1;
                    r++;
                }
            else
                {
                    if (pendente && w != s)
                        *w++ = ' ';
                    pendente = 0;
                    *w++ = *r++;
                }
        }
    *w = '\0'; /* discards pending trailing spaces */
}

char *
toDigits (Context *ctx)
{
    char *result = NULL, *aux = NULL, *ext = NULL;
    NumList *listHandle = ctx->list;
    uint16_t limit = orderOf (ctx->list), ord, proxOrd, proxClasse;
    uint16_t i, flare = 0, flag;
    if (limit)
        limit = (limit + 1 - MIL) * 3 + 3;
    else
        limit += 3;
    ext = (char *)alloc (limit * 2 + 1, sizeof (char));
    aux = ext;
    while (ctx->list && limit)
        {
            i = ctx->list->cls;
            if (i != CONJUCAO && i < MIL)
                {
                    ord = orderOf (ctx->list);
                    if (i < DEZ)
                        {
                            if (!flare)
                                {
                                    strcpy (aux, (const char *)"00");
                                    aux += 2;
                                    flare = 1;
                                    flag = 0;
                                }
                            if (aux - ext && (*(aux - 1) != '0' && *aux == '0' && *(aux + 1) == '0'))
                                aux++;
                            *aux++ = *(ctx->list->info->value);
                        }
                    else if (i < CEM)
                        {
                            if (!flare)
                                {
                                    strcpy (aux++, (const char *)"0");
                                    flare = 1;
                                    /*flag = 1;*/
                                }
                            strcpy (aux++, ctx->list->info->value);
                            flag = 1;
                        }
                    else
                        {
                            strcpy (aux++, ctx->list->info->value);
                            flare = 1;
                            flag = 2;
                        }
                }
            else if (i != CONJUCAO)
                {
                    flare = 0;
                    if (ctx->list->next)
                        proxOrd = orderOf (ctx->list->next);
                    else
                        proxOrd = NOVECENTOS;
                    while (ord - proxOrd >= 1)
                        {
                            flare = 1;
                            if (ord - proxOrd == 1)
                                {
                                    proxClasse = nextClass (ctx->list->next);
                                    if (proxClasse >= CEM)
                                        {
                                            uint16_t prevClass = ctx->list->prev->cls;
                                            if (prevClass >= CEM)
                                                aux += 2;
                                            else if (prevClass >= DEZ)
                                                aux++;
                                            break;
                                        }
                                    else if (proxClasse >= DEZ)
                                        {
                                            aux += flag;
                                            *aux++ = '0';
                                        }
                                    else if (proxClasse >= UM)
                                        {
                                            strcat (aux, "00");
                                            aux += 2 + flag;
                                        }
                                    else
                                        {
                                            strcat (aux, (const char *)"000");
                                            aux += 3 + flag;
                                        }
                                }
                            else
                                {
                                    strcat (aux, (const char *)"000");
                                    aux += 3;
                                }
                            proxOrd++;
                        }
                }
            ctx->list = ctx->list->next;
        }

    flare = strlen (ext);
    result = (char *)alloc (flare + 1, sizeof (char));
    strcpy (result, ext);
    free (ext);
    stripLeadingZeros (&result);
    ctx->list = listHandle;
    return result;
}

/*
**  Appends " " + the magnitude word for order `ord` to `result`:
**  ord 1 -> " mil"; ord >= 2 -> " milhao"/" milhoes" depending on `plural`.
**  ord 0 (units triplet) has no magnitude word.
*/
static void
appendMagnitude (Context *ctx, char *result, uint16_t ord, uint16_t plural)
{
    if (!ord)
        return;
    if (ord == 1) /* "mil" has no plural form */
        {
            char buf[MAXWLEN + 1];
            buf[0] = ' ';
            strcpy (buf + 1, ctx->dict->items[ord - 1 + MIL].name);
            strcat (result, buf);
            return;
        }
    char raw[MAXWLEN]; /* "milhao,milhoes" */
    strcpy (raw, ctx->dict->items[ord - 1 + MIL].name);
    char *comma = strchr (raw, ',');
    *comma = '\0';
    char buf[MAXWLEN + 1];
    buf[0] = ' ';
    strcpy (buf + 1, plural ? comma + 1 : raw);
    strcat (result, buf);
}

void
toWords (Context *ctx, char **answer)
{
    if (!**answer)
        {
            strcpy (*answer, (const char *)"zero");
            return;
        }
    uint16_t tam = strlen (*answer);
    if (tam > DECILHAO - 10)
        return;
    char *result = (char *)alloc (tam * 20, sizeof (char));
    char *aux = NULL;
    uint16_t ord, plural;
    int flag;
    while (tam > 0)
        {
            ord = (tam - 1) / 3;
            flag = toWordsTriplet (ctx, answer, result, &tam, &plural);
            tam = strlen (*answer);
            if (flag)
                {
                    appendMagnitude (ctx, result, ord, plural);
                    if ((**answer) && !((tam - 1) / 3))
                        {
                            strcat (result, (const char *)" e ");
                            ctx->isNumber = false;
                        }
                    strcat (result, " ");
                }
            if (ord == 1 && ctx->isNumber)
                {
                    uint16_t AC = 0, c = 0;
                    while ((*answer)[c])
                        AC += (*answer)[c++] - '0';
                    if (AC)
                        strcat (result, (const char *)" e ");
                }
        }
    aux = strrchr (result, 'e');
    if (aux && (*(aux - 1) == ' ' && *(aux + 1) == ' ') && (*(aux + 2) == ' ' || *(aux + 2) == '\0') && (*(aux + 3) == ' ' || *(aux + 3) == '\0'))
        *aux = '\0';
    normalizeSpaces (result);
    strcpy (*answer, result);
    free (result);
}

int
toWordsTriplet (Context *ctx, char **numberInput, char *result, uint16_t *size, uint16_t *flagPlural)
{
    char *currentNumber = *numberInput, label = 0, *tmp = NULL;
    uint16_t tam = *size, count = tam % 3;
    if (!count)
        count += 3;
    const uint16_t cnt = count;
    while (count)
        {
            label = 0;
            while (count && *currentNumber == '0')
                {
                    count--;
                    currentNumber++;
                }
            if (count)
                {
                    switch (count)
                        {
                        case 1:
                            ctx->isNumber = UM;
                            break;
                        case 2:
                            ctx->isNumber = VINTE;
                            label--;
                            break;
                        case 3:
                            ctx->isNumber = CEM;
                            break;
                        }
                    if (count == 2 && *currentNumber == '1')
                        {
                            label = 10;
                            currentNumber++;
                            ctx->isNumber = UM;
                            count--;
                        }
                    label += *currentNumber - '0';
                    tmp = (char *)alloc (25, sizeof (char));
                    strcpy (tmp, ctx->dict->items[label - 1 + ctx->isNumber].name);
                    if (strstr (tmp, (const char *)"cem"))
                        {
                            strcpy (tmp, (const char *)"cento");
                            if (currentNumber[1] == '0' && currentNumber[2] == '0')
                                {
                                    strcpy (tmp, (const char *)"cem");
                                    currentNumber += 2;
                                    count = 1;
                                }
                        }
                    strcat (result, tmp);
                    if (count != 1 && ((count == 3 && currentNumber[1] + currentNumber[2] != '0' + '0') || (count == 2 && currentNumber[1] != '0')))
                        strcat (result, (const char *)" e ");
                    count--;
                    currentNumber++;
                    free (tmp);
                }
            else if (*currentNumber)
                tam = strlen (currentNumber);
        }
    *flagPlural = (cnt == 1 && *(currentNumber - 1) == '1') ? 0 : 1;
    if (!*currentNumber)
        tam = 0;
    *numberInput = currentNumber;
    if (strcmp (currentNumber, (const char *)"000"))
        {
            tam -= cnt;
            *size = tam;
        }
    count = cnt;
    tam = 0;
    while (count)
        {
            tam += *(currentNumber - count) - '0';
            count--;
        }
    return (*currentNumber && tam);
}

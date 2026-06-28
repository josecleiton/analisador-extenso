#include "extenso/conversion.h"
#include "extenso/tokens.h"
#include "extenso/semantics.h"
#include "extenso/num_list.h"
#include "extenso/bignum.h"
#include "extenso/util.h"

/*
**  Normaliza o espaçamento da saída in-place: colapsa sequências de espaços em
**  um só e remove espaços no início e no fim. Garante saída limpa
**  independentemente dos separadores ad-hoc montados em toWords/toWordsTriplet.
*/
static void normalizeSpaces (char *s)
{
    char *r = s, *w = s;
    int pendente = 0;
    while (*r == ' ') r++;          /* pula espaços iniciais */
    while (*r)
    {
        if (*r == ' ')
        {
            pendente = 1;
            r++;
        }
        else
        {
            if (pendente && w != s) *w++ = ' ';
            pendente = 0;
            *w++ = *r++;
        }
    }
    *w = '\0';                       /* descarta espaços finais pendentes */
}

char *toDigits (Context *ctx)
{
    char *result = NULL, *aux = NULL, *ext = NULL;
    NumList* listHandle = ctx->list;
    uint16_t limit = orderOf(ctx->list), ord, proxOrd, proxClasse;
    uint16_t i, flare = 0, flag;
    if (limit) limit = (limit+1-MIL)*3+3;
    else limit+=3;
    ext = (char*) alloc (limit*2+1, sizeof (char));
    aux = ext;
    while (ctx->list && limit)
    {
        i = ctx->list -> cls;
        if (i != CONJUCAO && i < MIL)
        {
            ord = orderOf (ctx->list);
            if (i < DEZ)
            {
                if (! flare)
                {
                    strcpy (aux, (const char*) "00");
                    aux += 2;
                    flare = 1;
                    flag = 0;
                }
                if (aux - ext && (*(aux-1) != '0' && *aux == '0' && *(aux+1) == '0')) aux++;
                *aux++ = *(ctx->list -> info -> value);
            }
            else if (i < CEM)
            {
                if (! flare)
                {
                    strcpy (aux++, (const char*) "0");
                    flare = 1;
                    /*flag = 1;*/
                }
                strcpy (aux++, ctx->list -> info -> value);
                flag = 1;
            }
            else
            {
                strcpy (aux++, ctx->list -> info -> value);
                flare = 1;
                flag = 2;
            }
        }
        else if (i != CONJUCAO)
        {
            flare = 0;
            if (ctx->list -> next)
                proxOrd = orderOf (ctx->list -> next);
            else proxOrd = NOVECENTOS;
            while (ord - proxOrd >= 1)
            {
                flare = 1;
                if (ord - proxOrd == 1)
                {
                    proxClasse = nextClass (ctx->list -> next);
                    if (proxClasse >= CEM)
                    {
                        uint16_t prevClass = ctx->list -> prev -> cls;
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
                        strcat (aux, (const char*) "000");
                        aux += 3 + flag;
                    }
                }
                else
                {
                    strcat (aux, (const char*) "000");
                    aux += 3;
                }
                proxOrd++;
            }
        }
        ctx->list = ctx->list -> next;
    }

    flare = strlen (ext);
    result = (char*) alloc (flare + 1, sizeof (char));
    strcpy (result, ext);
    free (ext);
    stripLeadingZeros (&result);
    ctx->list = listHandle;
    return result;
}

void toWords (Context *ctx, char **answer)
{
    if (!**answer)
    {
        strcpy (*answer, (const char*) "zero");
        return;
    }
    uint16_t tam = strlen (*answer);
    if (tam > DECILHAO-10) return;
    char *result = (char*) alloc (tam*20, sizeof (char));
    char *aux = NULL;
    uint16_t ord, plural;
    int flag;
    while (tam > 0)
    {
        ord = (tam - 1)/3;
        flag = toWordsTriplet (ctx, answer, result, &tam, &plural);
        tam = strlen (*answer);
        if (flag)
        {
            if (ord == 1)
            {
                aux = (char*) alloc (5, sizeof (char));
                ++aux;
                strcpy (aux, ctx->dict->items[ord-1+MIL].name);
                *--aux = ' ';
                strcat (result, aux);
                free (aux);
            }
            else if (ord)
            {
                aux = (char*) alloc (36, sizeof (char));
                char* tmp = aux;
                ++aux;
                strcpy (aux, ctx->dict->items[ord-1+MIL].name);
                char* del = strchr (aux, ',');
                aux[del - aux] = '\0';
                if (plural)
                {
                    *--del = '\0';
                    aux = del+2;
                }
                *--aux = ' ';
                strcat (result, aux);
                free (tmp);
            }
            if ((**answer) && !((tam - 1)/3))
            {
                strcat (result, (const char*) " e ");
                ctx->isNumber = false;
            }
            strcat (result, " ");
        }
        if (ord==1 && ctx->isNumber)
        {
            uint16_t AC = 0, c = 0;
            while ((*answer)[c]) AC += (*answer)[c++] - '0';
            if (AC) strcat (result, (const char*) " e ");
        }
    }
    aux = strrchr (result, 'e');
    if (aux && (*(aux-1) == ' ' && *(aux+1) == ' ') && (*(aux+2) == ' ' || *(aux+2) == '\0') && (*(aux+3) == ' ' || *(aux+3) == '\0') ) *aux = '\0';
    normalizeSpaces (result);
    strcpy (*answer, result);
    free (result);
}

int toWordsTriplet (Context *ctx, char **numberInput, char *result, uint16_t *size, uint16_t *flagPlural)
{
    char *currentNumber = *numberInput, label = 0, *tmp = NULL;
    uint16_t tam = *size, count = tam%3;
    if (! count) count += 3;
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
                case 1: ctx->isNumber = UM; break;
                case 2: ctx->isNumber = VINTE; label--; break;
                case 3: ctx->isNumber = CEM; break;
            }
            if (count == 2 && *currentNumber == '1')
            {
                label = 10;
                currentNumber++;
                ctx->isNumber = UM;
                count--;
            }
            label += *currentNumber - '0';
            tmp = (char*) alloc (25, sizeof (char));
            strcpy (tmp, ctx->dict->items[label-1+ctx->isNumber].name);
            if (strstr (tmp, (const char*)"cem"))
            {
                strcpy (tmp, (const char*)"cento");
                if (currentNumber[1] == '0' && currentNumber[2] == '0')
                {
                    strcpy (tmp, (const char*) "cem");
                    currentNumber += 2;
                    count = 1;
                }
            }
            strcat (result, tmp);
            if (count != 1 && ((count==3 && currentNumber[1] + currentNumber[2] != '0'+'0') || (count==2 && currentNumber[1] != '0')))
                strcat (result, (const char*) " e ");
            count--;
            currentNumber++;
            free (tmp);
        }
        else if (*currentNumber) tam = strlen (currentNumber);
    }
    *flagPlural = (cnt == 1 && *(currentNumber-1) == '1') ? 0 : 1;
    if (!*currentNumber) tam = 0;
    *numberInput = currentNumber;
    if (strcmp (currentNumber, (const char*) "000"))
    {
        tam -= cnt;
        *size = tam;
    }
    count = cnt;
    tam = 0;
    while (count)
    {
        tam += *(currentNumber-count) - '0';
        count--;
    }
    return (*currentNumber && tam);
}

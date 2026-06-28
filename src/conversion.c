#include "extenso/conversion.h"
#include "extenso/state.h"
#include "extenso/tokens.h"
#include "extenso/semantics.h"
#include "extenso/num_list.h"
#include "extenso/bignum.h"
#include "extenso/util.h"

char *toNum (void)
{
    char *resultado = NULL, *aux = NULL, *ext = NULL;
    ListaNum* listHandle = list;
    uint16_t limit = pegaOrdem(list), ord, proxOrd, proxClasse;
    uint16_t i, flare = 0, flag;
    if (limit) limit = (limit+1-MIL)*3+3;
    else limit+=3;
    ext = (char*) alloc (limit*2+1, sizeof (char));
    aux = ext;
    while (list && limit)
    {
        i = list -> classe;
        if (i != CONJUCAO && i < MIL)
        {
            ord = pegaOrdem (list);
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
                *aux++ = *(list -> info -> valor);
            }
            else if (i < CEM)
            {
                if (! flare)
                {
                    strcpy (aux++, (const char*) "0");
                    flare = 1;
                    /*flag = 1;*/
                }
                strcpy (aux++, list -> info -> valor);
                flag = 1;
            }
            else
            {
                strcpy (aux++, list -> info -> valor);
                flare = 1;
                flag = 2;
            }
        }
        else if (i != CONJUCAO)
        {
            flare = 0;
            if (list -> prox)
                proxOrd = pegaOrdem (list -> prox);
            else proxOrd = NOVECENTOS;
            while (ord - proxOrd >= 1)
            {
                flare = 1;
                if (ord - proxOrd == 1)
                {
                    proxClasse = pegaProxClasse (list -> prox);
                    if (proxClasse >= CEM)
                    {
                        uint16_t prevClass = list -> ant -> classe;
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
        list = list -> prox;
    }

    flare = strlen (ext);
    resultado = (char*) alloc (flare + 1, sizeof (char));
    strcpy (resultado, ext);
    free (ext);
    trataZeros (&resultado);
    list = listHandle;
    return resultado;
}

void toName (char **resposta)
{
    if (!**resposta)
    {
        strcpy (*resposta, (const char*) "zero");
        return;
    }
    uint16_t tam = strlen (*resposta);
    if (tam > DECILHAO-10) return;
    char *resultado = (char*) alloc (tam*20, sizeof (char));
    char *aux = NULL;
    uint16_t ord, plural;
    int flag;
    while (tam > 0)
    {
        ord = (tam - 1)/3;
        flag = toNameMenOrd (resposta, resultado, &tam, &plural);
        tam = strlen (*resposta);
        fseek (dicionario, ind[ord-1+MIL], SEEK_SET);
        if (flag)
        {
            if (ord == 1)
            {
                aux = (char*) alloc (5, sizeof (char));
                fscanf (dicionario, "%[^=]", ++aux);
                *--aux = ' ';
                strcat (resultado, aux);
                free (aux);
            }
            else if (ord)
            {
                aux = (char*) alloc (36, sizeof (char));
                char* tmp = aux;
                fscanf (dicionario, "%[^=]", ++aux);
                char* del = strchr (aux, ',');
                aux[del - aux] = '\0';
                if (plural)
                {
                    *--del = '\0';
                    aux = del+2;
                }
                *--aux = ' ';
                strcat (resultado, aux);
                free (tmp);
            }
            if ((**resposta) && !((tam - 1)/3))
            {
                strcat (resultado, (const char*) " e ");
                flagNUM = false;
            }
            strcat (resultado, " ");
        }
        if (ord==1 && flagNUM)
        {
            uint16_t AC = 0, c = 0;
            while ((*resposta)[c]) AC += (*resposta)[c++] - '0';
            if (AC) strcat (resultado, (const char*) " e ");
        }
    }
    aux = strrchr (resultado, 'e');
    if (aux && (*(aux-1) == ' ' && *(aux+1) == ' ') && (*(aux+2) == ' ' || *(aux+2) == '\0') && (*(aux+3) == ' ' || *(aux+3) == '\0') ) *aux = '\0';
    strcpy (*resposta, resultado);
    free (resultado);
}

int toNameMenOrd (char **numberInput, char *resultado, uint16_t *size, uint16_t *flagPlural)
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
                case 1: flagNUM = UM; break;
                case 2: flagNUM = VINTE; label--; break;
                case 3: flagNUM = CEM; break;
            }
            if (count == 2 && *currentNumber == '1')
            {
                label = 10;
                currentNumber++;
                flagNUM = UM;
                count--;
            }
            label += *currentNumber - '0';
            fseek (dicionario, ind[label-1+flagNUM], SEEK_SET);
            tmp = (char*) alloc (25, sizeof (char));
            fscanf (dicionario, "%[^=]", tmp);
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
            strcat (resultado, tmp);
            if (count != 1 && ((count==3 && currentNumber[1] + currentNumber[2] != '0'+'0') || (count==2 && currentNumber[1] != '0')))
                strcat (resultado, (const char*) " e ");
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

#include "extenso/errors.h"
#include "extenso/config.h"
#include "extenso/state.h"
#include "extenso/util.h"
#include <time.h>

void erroSS (int tipoErro)
{
    FILE* erroS = openFile (ARQ_ERROS, "rb");
    char strErro[MAX_GEN], *strBump;
    int temp, i = 0, tamErro, tamEXP;
    uint16_t *idc = criaIndices (erroS, NUM_ERROS).index;
    fseek (erroS, idc[tipoErro], SEEK_SET);
    fgets (strErro, MAX_GEN, erroS);
    strcat (strErro, "\n\t");
    strcat (strErro, _TEXP);
    strcat (strErro, "\n\t");
    temp = EXP - _TEXP;
    tamErro = strlen (strErro);
    while (i < temp)
    {
        strErro[tamErro+i] = ' ';
        i++;
    }
    strErro[tamErro+i] = '^';
    strBump = &strErro[tamErro+i+1];
    for (tamEXP = strlen (_TEXP); tamEXP-temp; tamEXP--)
        *strBump++ = '~';
    *strBump++ = '\n';
    *strBump++ = '\0';
    char* toFile;
    uint16_t size_toFile = strlen(strErro)+50;
    time_t now;
    struct tm *timeinfo;
    time (&now);
    timeinfo = localtime (&now);
    toFile = (char*) alloc (size_toFile, sizeof (char));
    strcpy (toFile, asctime(timeinfo));
    char* needle = strchr (toFile, '\n');
    *++needle = '\0';
    strcat (toFile, strErro);
    needle = strrchr (toFile, '\n');
    *++needle = '\0';
    FILE* logs = openFile (ARQ_LOG, "at");
    fputs (toFile, logs);
    fflush (logs);
    fclose (logs);
    free (toFile);
    puts (strErro);
    free (idc);
    fclose (erroS);
    abortWithLog(false);
}

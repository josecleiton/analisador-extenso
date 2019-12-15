#include "interpretador.h"

#include <time.h>

#include "alloc.h"
#include "operacoes.h"

/*
**   Vários tokens que auxiliam na análise (léxica/semântica)
**   Se esses termos não forem familiares, leia README.md
*/
enum tokens {
   ZERO,
   UM,
   DOIS,
   TRES,
   QUATRO,
   CINCO,
   SEIS,
   SETE,
   OITO,
   NOVE,
   DEZ,
   ONZE,
   DOZE,
   TREZE,
   CATORZE,
   QUINZE,
   DEZESSEIS,
   DEZESSETE,
   DEZOITO,
   DEZENOVE,
   VINTE,
   TRINTA,
   QUARENTA,
   CINQUENTA,
   SESSENTA,
   SETENTA,
   OITENTA,
   NOVENTA,
   CEM,
   DUZENTOS,
   TREZENTOS,
   QUATROCENTOS,
   QUINHENTOS,
   SEISCENTOS,
   SETECENTOS,
   OITOCENTOS,
   NOVECENTOS,
   MIL,
   MILHAO,
   BILHAO,
   TRILHAO,
   QUATRILHAO,
   QUINTILHAO,
   SEXTILHAO,
   SETILHAO,
   OCTILHAO,
   NONILHAO,
   DECILHAO,
   CONJUCAO,
   NUM,
   DELIMITADOR
};

/*
**
**       VARIAVEIS GLOBAIS
**
*/
char* EXP; /* Ponteiro para expNum */
char* _TEXP; /* guarda a expressão sem modificações, para a possível exibição de
                erros */
char expNum[BUFSIZ]; /* Expressão que será analisada */
char token;          /* guarda o token */
short tipoToken;     /* sinalisa o tipo do token em analise */
unsigned flagNUM;    /* sinaliza se o(s) token(s) em análise são numeros */
uint16_t* ind; /* vetor que guarda as posições das strings no ARQ_DICT */
/* FILE* dicionario; /1* lista de tokens *1/ */
struct dict {
   const char key[MAXWLEN];
   const char value[MAXWLEN];
};
const struct dict dicionario[] = {
    {"zero", "0"},
    {"um", "1"},
    {"dois", "2"},
    {"tres", "3"},
    {"quatro", "4"},
    {"cinco", "5"},
    {"seis", "6"},
    {"sete", "7"},
    {"oito", "8"},
    {"nove", "9"},
    {"dez", "10"},
    {"onze", "11"},
    {"doze", "12"},
    {"treze", "13"},
    {"catorze", "14"},
    {"quinze", "15"},
    {"dezesseis", "16"},
    {"dezessete", "17"},
    {"dezoito", "18"},
    {"dezenove", "19"},
    {"vinte", "20"},
    {"trinta", "30"},
    {"quarenta", "40"},
    {"cinquenta", "50"},
    {"sessenta", "60"},
    {"setenta", "70"},
    {"oitenta", "80"},
    {"noventa", "90"},
    {"cem", "100"},
    {"duzentos", "200"},
    {"trezentos", "300"},
    {"quatrocentos", "400"},
    {"quinhentos", "500"},
    {"seiscentos", "600"},
    {"setecentos", "700"},
    {"oitocentos", "800"},
    {"novecentos", "900"},
    {"mil", "1000"},
    {"milhao,milhoes", "1000000"},
    {"bilhao,bilhoes", "1000000000"},
    {"trilhao,trilhoes", "1000000000000"},
    {"quatrilhao,quatrilhoes", "1000000000000000"},
    {"quintilhao,quintilhoes", "1000000000000000000"},
    {"sextilhao,sextilhoes", "1000000000000000000000"},
    {"setilhao,setilhoes", "1000000000000000000000000"},
    {"octilhao,octilhoes", "1000000000000000000000000000"},
    {"nonilhao,nonilhoes", "1000000000000000000000000000000"},
    {"decilhao,decilhoes", "1000000000000000000000000000000000"},
    {"e", "e"},
    {"abre-parentese", "("},
    {"fecha-parentese", ")"},
    {"mais", "+"},
    {"menos", "-"},
    {"vezes", "*"},
    {"dividido-por", "/"},
    {"mod", "%"},
    {"fatorial-de", "!"},
    {"elevado-a", "^"}};
ListaNum* list; /* guarda o número analisado por casas decimais em uma lista
                   encadeada */

#define MAX_GEN 512

void handBook(void) {
   printf(
       "A analise ocorre a partir de regras bem definidas, entao segue uma "
       "lista de comandos validos e instrucoes de uso:\n"
       "\n\nNUMEROS: \n"
       "A ordem e o plural importam. Nao utilize acentos nem pontuacao.\n"
       "dois milhoes = correto\n"
       "dois milhao = incorreto\n"
       "tres mil e quatro milhoes = incorreto\n"
       "quatro milhoes e tres mil = correto\n"
       "\n\nFORMATO DAS OPERACOES:\n\n"
       "Somar: numero mais numero\n"
       "Subtrair: numero menos numero\n"
       "Multiplicar: numero vezes numero\n"
       "Dividir: numero dividido por numero\n"
       "Resto: numero mod numero\n"
       "Exponencial: numero elevado a numero\n");
}

int fileParsingInit(void) {
   FILE* entrada = openFile(ARQ_ENTRADA, "rt");
   FILE* saida = openFile(ARQ_SAIDA, "wt");
   char* expOut = NULL; /* Resultado da expressão analisada */
   int count = 0;
   while (fgets(EXP, MAX_GEN, entrada)) {
      char* endline = strpbrk(EXP, "\r\n");
      if (endline) *endline = '\0';
      expOut = expParsingStart();
      fscanf(saida, "%s\n", expOut);
      fflush(saida);
      count += 1;
   }
   fclose(entrada);
   fclose(saida);
   /* fflush(stdout); */
   return count;
}

void printRes(void) {
   char op;
   printf("\nDeseja visualizar todas as expressoes resolvidas? (S/N)\n");
   scanf("%c%*c", &op);
   if (op == 'S' || op == 's' || op == '\n') {
      FILE* saida = openFile(ARQ_SAIDA, "rt");
      const size_t bufSize = BUFSIZ * 2;
      char* handle = (char*)alloc(bufSize, sizeof(char));
      printf("\n\tRESULTADOS (uma expressao por linha):\n\n");
      while (fgets(handle, bufSize, saida)) printf("%s", handle);
      free(handle);
      fclose(saida);
   }
}

char* expParsingStart(void) {
   strToLower();
   char* resposta = (char*)alloc(BUFSIZ * 2, sizeof(char));
   char* backupPtrResposta = resposta;
   /* dicionario = openFile(ARQ_DICT, "rb"); */
   /* ind = criaIndices(dicionario, TAM_DICT).index; */
   _TEXP = EXP;
   pegaToken();
   if (!token) erroSS(3);
   expResTermo(resposta);
   if (token) erroSS(0);
   toName(&resposta);
   // _TEXP = backupPtrResposta;
   free(ind);
   /* fclose(dicionario); */
   // free(_TEXP);
   return resposta;
}

void expResTermo(char* resposta) {
   register char op;
   register char* segTermo;
   expResFator(resposta);
   while ((op = token) == '+' || op == '-') {
      pegaToken();
      segTermo = (char*)alloc(_1KB, sizeof(char));
      expResFator(segTermo);
      switch (op) {
         case '-':
            memswap(resposta, segTermo, subtrair);
            break;
         case '+':
            memswap(resposta, segTermo, somar);
            break;
      }
      free(segTermo);
   }
}

void expResFator(char* resposta) {
   register char op;
   register char* segFator;
   expResFatorial(resposta);
   while ((op = token) == '*' || op == '/' || op == '%' || op == '^') {
      pegaToken();
      segFator = (char*)alloc(_1KB, sizeof(char));
      expResFatorial(segFator);
      switch (op) {
         case '*':
            memswap(resposta, segFator, multiplicar);
            break;
         case '/':
            memswapDiv(resposta, segFator, false, unsigneDiv);
            break;
         case '%':
            memswapDiv(resposta, segFator, true, unsigneDiv);
            break;
         case '^':
            memswap(resposta, segFator, unExpo);
            break;
      }
      if (*resposta == 'E') erroSS(13);
      free(segFator);
   }
}

void expResFatorial(char* resposta) {
   register char* proxFator;
   if (token == '!') {
      pegaToken();
      proxFator = (char*)alloc(_1KB, sizeof(char));
      expResParenteses(proxFator);
      char* temp = fatorial(proxFator);
      if (!temp) erroSS(8);
      strcpy(resposta, temp);
      free(proxFator);
      free(temp);
      if (!*resposta) erroSS(7);
      return;
   }
   expResParenteses(resposta);
}
/* IMPLEMENTAR ISSO DEPOIS
void expAvalSinal (char* resposta)
{
    register char op = 0;
    char* proxToken;
    char* tempPT;
    if ((tipoToken == DELIMITADOR) && *token=='+' || *token=='-')
    {
        tempPT = proxToken + 1;
        op = *token;
        getNumber (tempPT);
    }
    expResParenteses (resposta);
    if (op=='-')
    {
        *--tempPT = '-';
        strcpy (resposta, tempPT);
    }
}
*/

void expResParenteses(char* resposta) {
   if (token == '(') {
      pegaToken();
      expResTermo(resposta);
      if (token != ')') erroSS(1);
      pegaToken();
   } else
      atomo(resposta);
}

void atomo(char* resposta) {
   if (flagNUM) {
      if (analiSemantica()) {
         char* toNumAnswer = toNum();
         strcpy(resposta, toNumAnswer);
         free(toNumAnswer);
         listaLibera();
         flagNUM = false;
         pegaToken();
         return;
      }
      erroSS(3);
   }
   erroSS(0);
}

bool analiSemantica(void) {
   ListaNum* listSem = list;
   if (!listSem) erroSS(3);
   if (listaCount() > (DECILHAO - NOVECENTOS) * 4 - 1)
      erroSS(7); /* LIMITE DE DECILHÕES */
   uint16_t ord[2], i = 0;
   while (listSem) {
      pluralOrdem(listSem);
      semUnidade(&listSem);
      ord[i % 2] = pegaOrdem(listSem);
      if (i++ % 2 && ord[0] <= ord[1]) erroSS(2);
      if (listSem) listSem = listSem->prox;
   }
   return true;
}

void pluralOrdem(ListaNum* inicio) {
   ListaNum* aux = inicio;
   while (aux && (aux->classe < MILHAO || aux->classe == CONJUCAO))
      aux = aux->prox;
   if (aux && strstr(aux->info.nome, (const char*)"oes")) {
      if (inicio->classe != UM || inicio->classe == CONJUCAO) return;
   } else if (!aux || inicio->classe == UM || inicio->classe == CONJUCAO)
      return;
   erroSS(12);
}

bool semUnidade(ListaNum** inicio) {
   ListaNum* lista = *inicio;
   bool flag = false;
   while (lista && (lista->classe < MIL || lista->classe == CONJUCAO)) {
      if (lista->classe < VINTE && lista->classe != DEZ) {
         if (lista->prox) {
            if (lista->prox->classe == CONJUCAO) erroSS(11);
            if (lista->prox->classe < MIL) erroSS(2);
         }
      } else if (lista->classe == DEZ ||
                 (lista->classe >= VINTE && lista->classe <= NOVENTA)) {
         if (lista->prox &&
             (lista->prox->classe < MIL || lista->prox->classe == CONJUCAO)) {
            if (lista->prox->classe != CONJUCAO)
               erroSS(9);
            else if (lista->prox->prox == NULL)
               erroSS(10);
            else if (lista->prox->prox->classe > NOVE)
               erroSS(2);
         }
      } else if (lista->classe >= CEM && lista->classe <= NOVECENTOS) {
         if (!strcmp(lista->info.nome, (const char*)"cem") &&
             (lista->prox && lista->prox->classe == CONJUCAO))
            erroSS(12);
         if (lista->prox &&
             (lista->prox->classe < MIL || lista->prox->classe == CONJUCAO)) {
            if (lista->prox->classe != CONJUCAO)
               erroSS(9);
            else if (lista->prox->prox == NULL)
               erroSS(10);
            else if (lista->prox->prox->classe > NOVENTA)
               erroSS(2);
         }
      }
      lista = lista->prox;
      flag = true;
   }
   *inicio = lista;
   if (!flag) erroSS(5);
   return flag;
}

uint16_t pegaOrdem(ListaNum* inicio) {
   ListaNum* aux = inicio;
   while (aux && (aux->classe < MIL || aux->classe == CONJUCAO))
      aux = aux->prox;
   if (!aux) return NOVECENTOS;
   return aux->classe;
}

char* toNum(void) {
   char *resultado = NULL, *aux = NULL, *ext = NULL;
   ListaNum* listHandle = list;
   uint16_t limit = pegaOrdem(list), ord, proxOrd, proxClasse;
   uint16_t i, flare = 0, flag;
   if (limit)
      limit = (limit + 1 - MIL) * 3 + 3;
   else
      limit += 3;
   ext = (char*)alloc(limit * 2 + 1, sizeof(char));
   aux = ext;
   while (list && limit) {
      i = list->classe;
      if (i != CONJUCAO && i < MIL) {
         ord = pegaOrdem(list);
         if (i < DEZ) {
            if (!flare) {
               strcpy(aux, (const char*)"00");
               aux += 2;
               flare = 1;
               flag = 0;
            }
            if (aux - ext &&
                (*(aux - 1) != '0' && *aux == '0' && *(aux + 1) == '0'))
               aux++;
            *aux++ = *(list->info.valor);
         } else if (i < CEM) {
            if (!flare) {
               strcpy(aux++, (const char*)"0");
               flare = 1;
               /*flag = 1;*/
            }
            strcpy(aux++, list->info.valor);
            flag = 1;
         } else {
            strcpy(aux++, list->info.valor);
            flare = 1;
            flag = 2;
         }
      } else if (i != CONJUCAO) {
         flare = 0;
         if (list->prox)
            proxOrd = pegaOrdem(list->prox);
         else
            proxOrd = NOVECENTOS;
         while (ord - proxOrd >= 1) {
            flare = 1;
            if (ord - proxOrd == 1) {
               proxClasse = pegaProxClasse(list->prox);
               if (proxClasse >= CEM) {
                  uint16_t prevClass = list->ant->classe;
                  if (prevClass >= CEM)
                     aux += 2;
                  else if (prevClass >= DEZ)
                     aux++;
                  break;
               } else if (proxClasse >= DEZ) {
                  aux += flag;
                  *aux++ = '0';
               } else if (proxClasse >= UM) {
                  strcat(aux, "00");
                  aux += 2 + flag;
               } else {
                  strcat(aux, (const char*)"000");
                  aux += 3 + flag;
               }
            } else {
               strcat(aux, (const char*)"000");
               aux += 3;
            }
            proxOrd++;
         }
      }
      list = list->prox;
   }

   flare = strlen(ext);
   resultado = (char*)alloc(flare + 1, sizeof(char));
   strcpy(resultado, ext);
   free(ext);
   trataZeros(&resultado);
   list = listHandle;
   return resultado;
}

void erroSS(int tipoErro) {
   FILE* erroS = openFile(ARQ_ERROS, "rb");
   char strErro[MAX_GEN], *strBump;
   int temp, i = 0, tamErro, tamEXP;
   uint16_t* idc = criaIndices(erroS, NUM_ERROS).index;
   fseek(erroS, idc[tipoErro], SEEK_SET);
   fgets(strErro, MAX_GEN, erroS);
   strcat(strErro, "\n\t");
   strcat(strErro, _TEXP);
   strcat(strErro, "\n\t");
   temp = EXP - _TEXP;
   tamErro = strlen(strErro);
   while (i < temp) {
      strErro[tamErro + i] = ' ';
      i++;
   }
   strErro[tamErro + i] = '^';
   strBump = &strErro[tamErro + i + 1];
   for (tamEXP = strlen(_TEXP); tamEXP - temp; tamEXP--) *strBump++ = '~';
   *strBump++ = '\n';
   *strBump++ = '\0';
   char* toFile;
   uint16_t size_toFile = strlen(strErro) + 50;
   time_t now;
   struct tm* timeinfo;
   time(&now);
   timeinfo = localtime(&now);
   toFile = (char*)alloc(size_toFile, sizeof(char));
   strcpy(toFile, asctime(timeinfo));
   char* needle = strchr(toFile, '\n');
   *++needle = '\0';
   strcat(toFile, strErro);
   needle = strrchr(toFile, '\n');
   *++needle = '\0';
   FILE* logs = openFile(ARQ_LOG, "at");
   fputs(toFile, logs);
   fflush(logs);
   fclose(logs);
   free(toFile);
   puts(strErro);
   free(idc);
   fclose(erroS);
   abortWithLog(false);
}

Index criaIndices(FILE* in, int limite) {
   Index resultado;
   char handle[MAX_GEN];
   bool rlloc = false;
   if (!limite) {
      rlloc = true;
      limite = 32;
   }
   int i = 0;
   uint16_t* index = (uint16_t*)alloc(limite, sizeof(uint16_t));
   rewind(in);
   index[i++] = ftell(in);
   while (fgets(handle, MAX_GEN, in)) {
      index[i++] = ftell(in);
      if (i == limite && rlloc) {
         limite <<= 1;
         index = (uint16_t*)realloc(index, sizeof(uint16_t) * limite);
         if (!index) abortWithLog(true);
      }
   }
   index[--i] = 0;
   rewind(in);
   resultado.index = index;
   resultado.tam = i;
   return resultado;
}

uint16_t* _criaIndices(FILE* in, int size, int del) {
   rewind(in);
   uint16_t* index = (uint16_t*)alloc(size + 2, sizeof(uint16_t));
   uint16_t i = 1, k = 1;
   *index = 0;
   char ch = getc(in);
   while (ch != EOF && i <= size) {
      if (ch == del) index[i++] = k;
      ch = getc(in);
      k++;
   }
   rewind(in);
   return index;
}

void pegaToken(void) {
   const int dictLen = sizeof(dicionario) / sizeof(*dicionario);
   int cursorExp = 0;
   char trade = '\0';
   char valorTk = '\0';
   token = '\0';
   tipoToken = 0;
   if (!*EXP) return;
   while (isspace(*EXP)) ++EXP;
   while (EXP[cursorExp] && isalpha(EXP[cursorExp])) cursorExp++;
   trade = EXP[cursorExp];
   EXP[cursorExp] = '\0';
   ajustaDelim(&cursorExp, &trade);
   /* while (!feof(dicionario) && i < TAM_DICT) { */
   /*   fscanf(dicionario, "%[^=]=%[^\n]%*c", ref.nome, ref.valor); */
   char key[MAXWLEN] = {'\0'}, value[MAXWLEN]= {'\0'};
   for (int cursorDict = 0; cursorDict < dictLen; cursorDict += 1) {
      strcpy(key, dicionario[cursorDict].key);
      strcpy(value, dicionario[cursorDict].value);
      if (!strcmp(key, EXP) || resPlural(cursorDict, key)) {
         valorTk = value[0];
         if (isdigit(valorTk)) {
            token = valorTk;
            while (EXP[0] && (isalpha(EXP[0]) || EXP[0] == ' ')) EXP++;
            EXP[0] = trade;
            tipoToken = NUM;
            flagNUM = true;
            listaInsere(cursorDict, key, value);
            cursorDict = -1;
            if (verificaProxToken()) return;
         } else if (strchr(STR_DEL, valorTk)) {
            tipoToken = CONJUCAO;
            while (EXP[0] &&
                   (isalpha(EXP[0]) || EXP[0] == ' ' || EXP[0] == '-'))
               EXP++;
            token = valorTk;
            EXP[0] = trade;
            if (cursorDict != CONJUCAO) {
               tipoToken = DELIMITADOR;
               flagNUM = false;
               return;
            } else {
               listaInsere(cursorDict, key, value);
               cursorDict = -1;
               /* rewind(dicionario); */
            }
         }
      } else
         ajustaEXP();
   }
   erroSS(0);
}

void ajustaEXP(void) {
   while (*EXP && *EXP == ' ') EXP++;
   int k = strcspn(EXP, (const char*)" ");
   EXP[k] = '\0';
}

bool verificaProxToken(void) {
   while (EXP[0] && EXP[0] == ' ')
      EXP++; /* Posiciona a analise no inicio do proximo token */
   char* needle = strchr(EXP, ' ');
   if (!needle) {
      if (EXP[0]) return 0;
      return 1;
   }
   int k = needle - EXP;
   EXP[k] = '\0';
   int i = 1; /* COMEÇA EM UM PORQUE O PRIMEIRO DELIMITADOR É O 'e' */
   /* char DEL[20] = {'\0'}; */
   while (i < (TAM_DICT - INDEL + 1)) {
      const char* DEL = dicionario[INDEL+(i++)].key;
      /* fseek (dicionario, ind[INDEL+(i++)], SEEK_SET); */
      /* fscanf (dicionario, "%[^=]", DEL); */
      char* needle =
          strchr(DEL, '-'); /* TRATA O HIFEN NO DELIMITADOR COMPOSTO */
      if (needle) *needle = '\0';
      if (!strcmp(DEL, EXP)) {
         EXP[k] = ' ';
         /* rewind (dicionario); */
         return 1;
      }
   }
   EXP[k] = ' ';
   /* rewind (dicionario); */
   return 0;
}

bool resPlural(int i, char* currentToken) {
   if (!strchr("mbtqdscount", *currentToken)) return 0;
   const char* del = strchr(currentToken, ',');
   if (!del) return 0;
   bool fl = false;
   int k = del - currentToken;
   currentToken[k] = '\0';

   if (!strcmp(currentToken, EXP)) {
      fl = true;
   } else {
      ++del;
      if (!strcmp(del, EXP)) {
         char temp[MAXWLEN] = {'\0'};
         fl = true;
         strcpy(temp, del);
         strcpy(currentToken, temp);
         currentToken[++k] = '\0';
      }
   }
   return fl;
}

void ajustaDelim(int* k, char* temp) {
   if (*EXP != 'a' && *EXP != 'f' && *EXP != 'd' && *EXP != 'e')
      return;
   else if (!strcmp(EXP, (const char*)"abre") ||
            !strcmp(EXP, (const char*)"fecha") ||
            !strcmp(EXP, (const char*)"dividido") ||
            !strcmp(EXP, (const char*)"fatorial") ||
            !strcmp(EXP, (const char*)"elevado")) {
      int i = 0;
      EXP[*k] = '-';
      while (isalpha(EXP[i]) || EXP[i] == '-') i++;
      *temp = EXP[i];
      EXP[i] = '\0';
      if (strcmp(&EXP[*k + 1], (const char*)"parentese") &&
          strcmp(&EXP[*k + 1], (const char*)"de") &&
          strcmp(&EXP[*k + 1], (const char*)"por") &&
          strcmp(&EXP[*k + 1], (const char*)"a")) {
         EXP[*k] = ' ';
         erroSS(0);
         return;
      }
   }
}

void toName(char** resposta) {
   if (!**resposta) {
      strcpy(*resposta, (const char*)"zero");
      return;
   }
   uint16_t tam = strlen(*resposta);
   if (tam > DECILHAO - 10) return;
   char* resultado = (char*)alloc(tam * 20, sizeof(char));
   char aux[40] = {'\0'};
   uint16_t ord, plural;
   int flag;
   while (tam > 0) {
      ord = (tam - 1) / 3;
      flag = toNameMenOrd(resposta, resultado, &tam, &plural);
      tam = strlen(*resposta);
      /* fseek (dicionario, ind[ord-1+MIL], SEEK_SET); */
      if (flag) {
         if (ord == 1) {
            /* aux = (char*) alloc (5, sizeof (char)); */
            /* fscanf (dicionario, "%[^=]", ++aux); */
            sprintf(aux, " %s", dicionario[ord - 1 + MIL].key);
            strcat(resultado, aux);
         } else if (ord) {
            /* aux = (char*) alloc (36, sizeof (char)); */
            sprintf(aux, " %s", dicionario[ord - 1 + MIL].key);
            char* tmp = aux;
            /* fscanf (dicionario, "%[^=]", ++aux); */
            char* del = strchr(aux, ',');
            aux[del - tmp] = '\0';
            if (plural) {
               *--del = '\0';
               tmp = del + 2;
            }
            *--tmp = ' ';
            strcat(resultado, tmp);
         }
         if ((**resposta) && !((tam - 1) / 3)) {
            strcat(resultado, (const char*)" e ");
            flagNUM = false;
         }
         strcat(resultado, " ");
      }
      if (ord == 1 && flagNUM) {
         uint16_t AC = 0, c = 0;
         while ((*resposta)[c]) AC += (*resposta)[c++] - '0';
         if (AC) strcat(resultado, (const char*)" e ");
      }
   }
   char* tmp = strrchr(resultado, 'e');
   if (tmp && (*(tmp - 1) == ' ' && *(tmp + 1) == ' ') &&
       (*(tmp + 2) == ' ' || *(tmp + 2) == '\0') &&
       (*(tmp + 3) == ' ' || *(tmp + 3) == '\0'))
      *tmp = '\0';
   strcpy(*resposta, resultado);
   free(resultado);
}

int toNameMenOrd(char** numberInput, char* resultado, uint16_t* size,
                 uint16_t* flagPlural) {
   char *currentNumber = *numberInput, label = 0, tmp[32] = {'\0'};
   uint16_t tam = *size, count = tam % 3;
   if (!count) count += 3;
   const uint16_t cnt = count;
   while (count) {
      label = 0;
      while (count && *currentNumber == '0') {
         count--;
         currentNumber++;
      }
      if (count) {
         switch (count) {
            case 1:
               flagNUM = UM;
               break;
            case 2:
               flagNUM = VINTE;
               label--;
               break;
            case 3:
               flagNUM = CEM;
               break;
         }
         if (count == 2 && *currentNumber == '1') {
            label = 10;
            currentNumber++;
            flagNUM = UM;
            count--;
         }
         label += *currentNumber - '0';
         strcpy(tmp, dicionario[label - 1 + flagNUM].key);
         if (strstr(tmp, (const char*)"cem")) {
            strcpy(tmp, (const char*)"cento");
            if (currentNumber[1] == '0' && currentNumber[2] == '0') {
               strcpy(tmp, (const char*)"cem");
               currentNumber += 2;
               count = 1;
            }
         }
         strcat(resultado, tmp);
         if (count != 1 &&
             ((count == 3 &&
               currentNumber[1] + currentNumber[2] != '0' + '0') ||
              (count == 2 && currentNumber[1] != '0')))
            strcat(resultado, (const char*)" e ");
         count--;
         currentNumber++;
      } else if (*currentNumber)
         tam = strlen(currentNumber);
   }
   *flagPlural = (cnt == 1 && *(currentNumber - 1) == '1') ? 0 : 1;
   if (!*currentNumber) tam = 0;
   *numberInput = currentNumber;
   if (strcmp(currentNumber, (const char*)"000")) {
      tam -= cnt;
      *size = tam;
   }
   count = cnt;
   tam = 0;
   while (count) {
      tam += *(currentNumber - count) - '0';
      count--;
   }
   return (*currentNumber && tam);
}

void listaInsere(uint16_t i, const char* nome, const char* valor) {
   ListaNum* no = (ListaNum*)alloc(1, sizeof(ListaNum));
   ListaNum* aux = list;
   // no->info = (Ordem*)alloc(1, sizeof(Ordem));
   strcpy(no->info.nome, nome);
   strcpy(no->info.valor, valor);
   no->classe = i;
   // no->prox = NULL;
   if (!list) {
      list = no;
      // no->ant = NULL;
      return;
   }
   while (aux && aux->prox) aux = aux->prox;
   no->ant = pegaProxNum(list);
   aux->prox = no;
}

uint16_t pegaProxClasse(ListaNum* inicio) {
   uint16_t classe = 0;
   if (!inicio) return classe;
   while (inicio->classe >= MIL) inicio = inicio->prox;
   classe = inicio->classe;
   return classe;
}

ListaNum* pegaProxNum(ListaNum* inicio) {
   ListaNum* aux = NULL;
   while (inicio) {
      if (inicio->classe < MIL) aux = inicio;
      inicio = inicio->prox;
   }
   return aux;
}

void listaLibera(void) {
   ListaNum *aux = list, *zombie = NULL;
   while (aux) {
      zombie = aux;
      aux = aux->prox;
      // if (zombie->info) free(zombie->info);
      free(zombie);
   }
   list = NULL;
}

int listaCount(void) {
   ListaNum* aux;
   int n;
   for (n = 0, aux = list; aux; aux = aux->prox, n++)
      ;
   return n;
}

int fstrcount(FILE* in) {
   rewind(in);
   int i = 1;
   char ch = getc(in);
   while (ch != EOF) {
      if (ch == '\n') i++;
      ch = getc(in);
   }
   return i;
   rewind(in);
}

void clearScreen(void) {
   int n;
   for (n = 0; n < 10; n++) printf("\n\n\n\n\n\n\n\n\n\n");
}

void strToLower(void) {
   int i = 0;
   while (EXP[i]) {
      EXP[i] = tolower(EXP[i]);
      i++;
   }
}


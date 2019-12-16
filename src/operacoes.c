#include "operacoes.h"

#include <math.h>

bool inverte(char a[]) {
   char c;
   for (int i = 0, tam = strlen(a); i < tam / 2; i++) {
      c = a[i];
      a[i] = a[tam - 1 - i];
      a[tam - 1 - i] = c;
   }
   return true;
}

bool char2int(char a[]) {
   int i, tam = strlen(a);
   for (i = 0; i <= tam; i++) a[i] = a[i] - '0';
   return true;
}

bool int2char(char a[], int tam) {
   int i;
   for (i = 0; i < tam - 1; i++) {
      a[i] += '0';
   }
   if (!isdigit(a[i - 1])) a[i - 1] = '\0';
   a[i] = '\0';
   return true;
}

int maior(int a, int b) { return (a >= b) * a + (b > a) * b; }

int menor(int a, int b) { return (a <= b) * a + (b < a) * b; }

bool strIsDigit(char a[]) {
   int i = 0;
   while (a[i]) {
      if (!isdigit(a[i]) && !(a[i] >= 0 && a[i] <= 9)) return false;
      i++;
   }
   return true;
}

char* somar(char a[], char b[]) {
   int ta = strlen(a), tb = strlen(b), ts = maior(ta, tb);
   if (!ta)
      return b;
   else if (!tb)
      return a;
   int i, vaium = 0;
   char *soma, *op2, *op1, flag = 0;
   op1 = (ta >= tb) ? a : b;
   op2 = completaMenor(a, b, &flag);
   bool allocated = true;
   if (!op2) {
      allocated = false;
      op2 = (ta >= tb) ? b : a;
   }
   soma = (char*)alloc(ts + 2, sizeof(char));
   *soma = '0';
   soma++;
   for (i = ts - 1; i >= 0; i--) {
      soma[i] = (op1[i] - '0') + (op2[i] - '0') + vaium;
      vaium = 0;
      if (soma[i] >= 10) {
         soma[i] %= 10;
         vaium = 1;
      }
      soma[i] += '0';
   }
   *--soma += vaium;
   trataZeros(&soma);
   if (allocated) free(op2);
   return soma;
}

char* subtrair(char a[], char b[]) {
   char *min = NULL, *subt = NULL, flagSinal,
        flagMenor; /* flagMenor = [ se menor == 1, então a string a é menor; se
                      menor == 0, então string a é maior; se menor == -1, ambas
                      têm o mesmo tamanho ] */
   int i;
   int tamMinuendo = strlen(a), tamSubtraendo = strlen(b);
   bool allocated = false;
   if (!tamMinuendo)
      return b;
   else if (!tamSubtraendo)
      return a;
   if (tamMinuendo != tamSubtraendo) {
      subt = completaMenor(a, b, &flagMenor);
      allocated = true;
      if (flagMenor) {
         min = b;
         flagSinal = 1;
      } else {
         min = a;
         flagSinal = 0;
      }
   } else {
      if (strcmp(a, b) >= 0) {
         min = a;
         subt = b;
         flagSinal = 0;
      } else {
         min = b;
         subt = a;
         flagSinal = 1;
      }
   }
   tamMinuendo = strlen(min);
   tamSubtraendo = strlen(subt);
   char* diferenca = (char*)alloc(tamMinuendo + 1, sizeof(char));
   for (i = tamMinuendo - 1; i >= 0; i--) {
      diferenca[i] = (min[i] - '0') - (subt[i] - '0');
      if (diferenca[i] < 0) {
         if (i) min[i - 1]--;
         diferenca[i] += 10;
      }
      diferenca[i] += '0';
   }
   trataZeros(&diferenca);
   if (allocated) free(subt);
   return diferenca;
}

char* completaMenor(char a[], char b[], char* menor) {
   int tamMaior = 0, tamMenor = 0, tamA = strlen(a), tamB = strlen(b);
   char __menor;
   int k = 0;
   if (tamA > tamB) {
      tamMaior = tamA;
      tamMenor = tamB;
      __menor = 0;
   } else if (tamA < tamB) {
      tamMaior = tamB;
      tamMenor = tamA;
      __menor = 1;
   }
   if (tamMaior + tamMenor) {
      char* completaZeros = (char*)alloc(tamMaior + 1, sizeof(char));
      while (k < tamMaior - tamMenor) {
         completaZeros[k] = '0';
         k++;
      }
      completaZeros[tamMaior - tamMenor] = '\0';

      if (__menor) {
         strcat(completaZeros, a);
         *menor = __menor;
      } else {
         strcat(completaZeros, b);
         *menor = __menor;
      }
      return completaZeros;
   } else {
      *menor = -1;
      return NULL;
   }
}

char* multiplicar(char a[], char b[]) {
   uint16_t ta = strlen(a);
   uint16_t tb = strlen(b);
   if ((ta == 0) || (tb == 1 && *b == '1'))
      return a;
   else if ((tb == 0) || (ta == 1 && *a == '1'))
      return b;
   char* produto = (char*)alloc(ta + tb + 10, sizeof(char));
   int ls = 0, i, j, cursor = 0;
   for (i = tb - 1; i >= 0; i--) {
      int carry = 0, k = ls;
      for (j = ta - 1; j >= 0; j--) {
         int temp = (a[j] - '0') * (b[i] - '0') + carry;
         if (produto[k]) temp += produto[k] - '0';
         produto[k++] = temp % 10 + '0';
         carry = temp / 10;
      }
      if (carry > 0) produto[k] = carry + '0';
      ls++;
      cursor = k;
   }
   inverte(produto);
   return produto;
}

char* unExpo(char a[], char b[]) {
   char* answer = NULL;
   if (*b == '0' || *b == '\0') {
      answer = (char*)alloc(2, sizeof(char));
      answer[0] = '1';
      answer[1] = '\0';
      return answer;
   } else if (*b == '-')
      return answer;  // NULL
   int lenA = strlen(a), lenB = strlen(b);
   answer = (char*)alloc(lenB * 10 * lenA * 2, sizeof(char));
   strcpy(answer, a);
   while (strcmp(b, "1")) {
      memswap(answer, a, multiplicar);
      memswap(b, "1", subtrair);
   }
   return answer;
}

/*
**  DIVISÃO POSITIVA
**
**  N = Numerador
**  Q = Quociente
**
**  O algoritmo de divisão foi baseado na divisão do Ensino Fundamental
**
**  15467 | 58      Selecione em N o tamanho de D
**   386  |_____    -- 15
**    387  266      A subtração de 15 por 58 (D) resulta em um número positivo?
**      39           -- Não
**                  Então pegue o proximo numero de N e coloque em 15
**                  -- 154
**                  subtraia 154 por 58 até que gerar um resultado negativo
**                  -- 154 - 58 = 96
**                  -- 96 - 58 = 38
**                  Coloque em Q quantas vezes a subtração ocorreu
**                  -- Q(0) := 2
**                  Coloque em 38 o proximo digito de N
**                  -- 386
**                  Faça a subtração novamente por D
**                  Coloque em Q quantas vezes a subtração ocorreu
**                  -- Q(1) := 6
**                  [...]
**                  O algoritmo segue até termos
**                        266 em Q
**                        39 em N (RESTO)
**
**
**  A função abaixo aplica o algoritmo acima, nele podemos visualizar que
**  Q sempre terá len(N) - len(D) digitos
**
*/
char* unsigneDiv(char a[], char D[], bool MOD)  // TRATAR OS ZEROS 500000/20
{
   const int tn = strlen(a),
             td = strlen(D); /* len(N) e len(D) respectivamente */
   if (td > tn) {
      if (MOD) return a; /* 2 % 3 == 2 */
      return NULL;       /* 2 / 3 == 0 */
   }
   if (!td) return (char*)"E";         /* divisão por zero é indeterminada */
   if (*D == '1' && td == 1) return a; /* divisão por um */
   if (!strcmp(a, D)) {
      /* divisão de numeros iguais */
      char* um = (char*)alloc(2, sizeof(char));
      *um = '1';
      return um;
   }
   if (!tn) return a;
   /* N, Q e o ponteiro que guarda o inicio da alocação primeira de N */
   char* N = (char*)alloc(tn + 1, sizeof(char));
   strcpy(N, a);
   char* Q = (char*)alloc(
       tn - td + 2,
       sizeof(char)); /* O quociente terá pelo menos tn-td+1 digitos */
   char* temp = N;
   int i;            /* indice de interações do laço para a divisão */
   int j = 0, k = 0; /* cursor para escrita na string N e Q*/
   int l = 0;        /* cursor na string de entrada a */
   size_t countSub;  /* conta quantas subtrações foram feitas de N por D */
   int leN;          /* guarda o tamanho atualizado (pela subtração) de N */
   // bool fl = false; /* Marca se ocorreu ou não uma subtração de N por D */;
   char handle;
   for (i = 0; k < tn - td + 1 && j < tn - td + 1; i++) {
      handle = '\0';
      charswap(&N[td + j], &handle);
      leN = strlen(N);
      countSub = 0ull;
      while (strCmpNum(N, D)) {
         memswap(N, D, subtrair);
         countSub++;
      }
      if (countSub) {
         if (j) k--; /* SE A MOVIMENTAÇÃO NÃO GEROU UM 0, RETIRE-O */
         if (countSub < 10)
            Q[k++] = countSub + '0';
         else {
            int w = countDigits(countSub);
            for (w--; w >= 0; w--) {
               Q[k++] = (countSub / (int)pow(10, w)) + '0';
               countSub %= (int)pow(10, w);
            }
         }
         strcpy(&N[td + j + strlen(N) - leN], &a[td + 1 + l++]);
         j = 0;
      } else {
         Q[k++] = '0';
         charswap(&N[td + j++], &handle);
      }
   }
   if (MOD) strcpy(Q, N);
   free(temp);
   return Q;
}

bool strCmpNum(char x[], char y[]) {
   char *a = x, *b = y;
   ignoraZero(2, &a, &b);
   int ta = strlen(a), tb = strlen(b);
   if (ta > tb) return true;
   if (ta == tb) {
      int i;
      for (i = 0; a[i]; i++) {
         if (a[i] > b[i])
            return true;
         else if (a[i] < b[i])
            return false;
      }
   } else
      return false;
   return true;
}

int countDigits(long long x) { return (int)floor(log10(x)) + 1; }

char* fatorial(char in[]) {
   char* inTemp = (char*)alloc(strlen(in) + 1, sizeof(char));
   register char* fat = (char*)alloc(_1KB, sizeof(char));
   strcpy(inTemp, in);
   int i = 0, k = 1, tamA = strlen(inTemp);
   if (tamA > 3) return NULL;
   i = 0;
   *fat = 1;
   long long num = 0ll;
   inverte(inTemp);
   char2int(inTemp);
   while (i < tamA) {
      num += (long long)inTemp[i] * pow(10, i);
      i++;
   }
   free(inTemp);
   if (num > 400 || num < 0) return NULL;
   for (i = 2; i <= num; i++) k = fatorialMultiplicador(i, fat, k);
   int2char(fat, k + 1);
   inverte(fat);
   return fat;
}

int fatorialMultiplicador(int a, char fat[], int limit) {
   int count, produto, resto = 0;
   for (count = 0; count < limit; count++) {
      produto = fat[count] * a + resto;
      fat[count] = produto % 10;
      resto = produto / 10;
   }
   while (resto) {
      fat[limit] = resto % 10;
      resto = resto / 10;
      limit++;
   }
   return limit;
}

void trataZeros(char** ptrNumber) {
   char* number = *ptrNumber;
   int x;
   for (x = 0; *number == '0'; x++) number++;
   if (!x) return;
   int len = strlen(number);
   char* newNumber = (char*)alloc(len + 1, sizeof(char));
   strcpy(newNumber, number);
   free(number - x);
   *ptrNumber = newNumber;
}

void ignoraZero(int narg, ...) {
   va_list argList;
   va_start(argList, narg);
   char** arg;
   while (narg--) {
      arg = va_arg(argList, char**);
      while (**arg == '0') (*arg)++;
   }
   va_end(argList);
}

bool memswap(char a[], char b[], char* (*f)(char*, char*)) {
   char* temp = f(a, b);
   if (temp) {
      strcpy(a, temp);
      free(temp);
   } else
      *a = '\0';
   return (a != NULL);
}

bool memswapDiv(char a[], char b[], bool mod, char* (*f)(char*, char*, bool)) {
   char* temp = f(a, b, mod);
   if (temp) {
      strcpy(a, temp);
      free(temp);
   } else
      *a = '\0';
   return (a != NULL);
}

void charswap(char* a, char* b) {
   register char h = *a;
   *a = *b;
   *b = h;
}

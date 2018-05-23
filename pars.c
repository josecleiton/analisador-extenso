
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
   
#define DELIMITADOR 1
#define VARIAVEL 2
#define NUMERO 3

char *prog;
char token[80];
char tok_type;
             
void eval_exp(double *answer);
void eval_exp2(double *answer);
void eval_exp3(double *answer);
void eval_exp4(double *answer);
void eval_exp5(double *answer);
void eval_exp6(double *answer);
void atom(double *answer);
void get_token(void);
void putback(void);
void unary(char o, double *r);
void serror(int error);
int isdelim(char c);

void eval_exp(double *answer)
{
   get_token();
   if(!token)
   {
      serror(2);
      return;
   }
   eval_exp2(answer);
}

void eval_exp2(double *answer)
{
   register char op;
   double temp;

   eval_exp3(answer);
   while ( (op = *token) == '+' || op == '-')
   {
      get_token();
      eval_exp3(&temp);
      switch(op) {
         case '-':
            *answer = *answer - temp;
            break;
         case '+' :
            *answer = *answer + temp;
            break;
      }
   }
}

void eval_exp3(double *answer)
{
   register char op;
   double temp;

   eval_exp4(answer);

   while ((op=*token) == '%' || op == '/' || op =='*')
   {
      get_token();
      eval_exp4(&temp);
      switch (op) {
         case '*':
            *answer = *answer * temp;
            break;
         case '/':
            *answer = *answer / temp;
            break;

         case '%':
            *answer = (int)*answer % (int)temp;
            break;
      }
   }
}

void eval_exp4(double *answer)
{
   double temp, ex;
   register int t;

   eval_exp5(answer);
   if (*token=='^') {
      get_token();
      eval_exp4(&temp);
      ex = *answer;
      if (temp==0.0) {
         *answer = 1.0;
         return;
      }
      for (t=(int)temp-1;t>0;--t) *answer = (*answer) * (double)ex;
   }
}

void eval_exp5(double *answer)
{
   register char op;
   op = 0;
   if ((tok_type == DELIMITADOR) && *token=='+' || *token=='-') {
      op = *token;
      get_token();
   }
   eval_exp6(answer);
   if (op=='-') *answer = -(*answer);
}

void eval_exp6(double *answer)
{
   if ((*token=='(')) {
      get_token();
      eval_exp2(answer);
      if (*token != ')')
         serror(1);
      get_token();
   }
   else
      atom(answer);
}
void atom(double *answer)
{
   if (tok_type == NUMERO) {
      *answer = atof(token);
      get_token();
      return;
   }
   serror(0);
}

void putback()
{
 char *t;
 t = token;
 for (;*t;t++) prog--;
}

void serror(int error)
{
   static char *e[] = {"erro de sintaxe",
                       "falta parenteses",
                       "nenhuma expressao presente"
                      };
   printf("%s\n", e[error]);
}

void get_token(void)
{
   register char *temp;
   tok_type = 0;
   temp = token;
   *temp = '\0';
   if (!*prog) return;
   while (isspace(*prog)) ++prog;
   if (strchr("+-*/%^=()", *prog)) {
      tok_type = DELIMITADOR;
      *temp++ = *prog++;
   }
   else if (isdigit(*prog)) {
        while (!isdelim(*prog)) *temp++ = *prog++;
        tok_type = NUMERO;
   }
   *temp = '\0';
}

int isdelim(char c)
{

   if (strchr(" +-*/%^=()", c) || c==9 || c=='\r' || c==0) {
      return 1;
   }
   return 0;
}

main(void)
{
   double answer;
   char *p;

   p = (char *) calloc(100, sizeof(p));
   prog = (char *) calloc(100, sizeof(prog));
 
   if (!p) {
      printf("falha na execucao\n");
      exit(1);
   }

   printf("Entre com uma expressao usando: +, -, *, /, %, ^, ( ou ). Por exemplo: 2+3-4*5/2^10 e tecle ENTER\n\n");
   do {
      printf("> ");
      scanf("%s", prog);
      if (!*prog) break;
      eval_exp(&answer);
      printf(">> %.15f\n\n", answer);
   } while (!*p);
} 

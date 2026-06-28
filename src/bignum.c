#include "extenso/bignum.h"

bool
reverseStr (char a[])
{
    char c;
    int i, tam = strlen (a);
    for (i = 0; i < tam / 2; i++)
        {
            c = a[i];
            a[i] = a[tam - 1 - i];
            a[tam - 1 - i] = c;
        }
    return true;
}

bool
digitsToValues (char a[])
{
    int i, tam = strlen (a);
    for (i = 0; i <= tam; i++)
        a[i] = a[i] - '0';
    return true;
}

bool
valuesToDigits (char a[], int tam)
{
    int i;
    for (i = 0; i < tam - 1; i++)
        {
            a[i] += '0';
        }
    if (!isdigit (a[i - 1]))
        a[i - 1] = '\0';
    a[i] = '\0';
    return true;
}

int
maior (int a, int b)
{
    return (a >= b) * a + (b > a) * b;
}

int
menor (int a, int b)
{
    return (a <= b) * a + (b < a) * b;
}

bool
isAllDigits (char a[])
{
    int i = 0;
    while (a[i])
        {
            if (!isdigit (a[i]) && !(a[i] >= 0 && a[i] <= 9))
                return false;
            i++;
        }
    return true;
}

char *
bigAdd (char a[], char b[])
{
    int ta = strlen (a), tb = strlen (b), ts = maior (ta, tb);
    if (!ta)
        return b;
    else if (!tb)
        return a;
    int i, vaium = 0;
    char *soma, *op2, *op1, flag = 0;
    op1 = (ta >= tb) ? a : b;
    op2 = padToWidth (a, b, &flag);
    bool allocated = true;
    if (!op2)
        {
            allocated = false;
            op2 = (ta >= tb) ? b : a;
        }
    soma = (char *)alloc (ts + 2, sizeof (char));
    *soma = '0';
    soma++;
    for (i = ts - 1; i >= 0; i--)
        {
            soma[i] = (op1[i] - '0') + (op2[i] - '0') + vaium;
            vaium = 0;
            if (soma[i] >= 10)
                {
                    soma[i] %= 10;
                    vaium = 1;
                }
            soma[i] += '0';
        }
    *--soma += vaium;
    stripLeadingZeros (&soma);
    if (allocated)
        free (op2);
    return soma;
}

/*
**  Unsigned subtraction: returns |a - b|. Does not mutate the inputs (the
**  original version decremented the minuend during the "borrow"); uses a
**  borrow variable.
*/
char *
bigSub (char a[], char b[])
{
    char *min = NULL, *subt = NULL, flagMenor;
    int i;
    int tamA = strlen (a), tamB = strlen (b);
    bool allocated = false;
    if (!tamA)
        return b;
    else if (!tamB)
        return a;
    if (tamA != tamB)
        {
            subt = padToWidth (a, b, &flagMenor); /* zero-pad the smaller one */
            allocated = true;
            min = flagMenor ? b : a; /* min = the larger (longer) one */
        }
    else
        {
            if (strcmp (a, b) >= 0)
                {
                    min = a;
                    subt = b;
                }
            else
                {
                    min = b;
                    subt = a;
                }
        }
    int tam = strlen (min);
    char *difference = (char *)alloc (tam + 1, sizeof (char));
    int borrow = 0;
    for (i = tam - 1; i >= 0; i--)
        {
            int d = (min[i] - '0') - (subt[i] - '0') - borrow;
            if (d < 0)
                {
                    d += 10;
                    borrow = 1;
                }
            else
                borrow = 0;
            difference[i] = (char)(d + '0');
        }
    stripLeadingZeros (&difference);
    if (allocated)
        free (subt);
    return difference;
}

char *
padToWidth (char a[], char b[], char *menor)
{
    int tamMaior = 0, tamMenor = 0, tamA = strlen (a), tamB = strlen (b);
    char __menor;
    int k = 0;
    if (tamA > tamB)
        {
            tamMaior = tamA;
            tamMenor = tamB;
            __menor = 0;
        }
    else if (tamA < tamB)
        {
            tamMaior = tamB;
            tamMenor = tamA;
            __menor = 1;
        }
    if (tamMaior + tamMenor)
        {
            char *completaZeros = (char *)alloc (tamMaior + 1, sizeof (char));
            while (k < tamMaior - tamMenor)
                {
                    completaZeros[k] = '0';
                    k++;
                }
            completaZeros[tamMaior - tamMenor] = '\0';

            if (__menor)
                {
                    strcat (completaZeros, a);
                    *menor = __menor;
                }
            else
                {
                    strcat (completaZeros, b);
                    *menor = __menor;
                }
            return completaZeros;
        }
    else
        {
            *menor = -1;
            return NULL;
        }
}

/* Removes leading zeros in-place, keeping at least one digit ("0"). */
static void
bnStrip (char *s)
{
    char *p = s;
    while (p[0] == '0' && p[1] != '\0')
        p++;
    if (p != s)
        memmove (s, p, strlen (p) + 1);
}

/* Schoolbook multiplication O(n*m). */
static char *
mulSchool (const char *a, const char *b)
{
    int ta = strlen (a), tb = strlen (b);
    char *product = (char *)alloc (ta + tb + 2, sizeof (char));
    int ls = 0, i, j;
    for (i = tb - 1; i >= 0; i--)
        {
            int carry = 0, k = ls;
            for (j = ta - 1; j >= 0; j--)
                {
                    int temp = (a[j] - '0') * (b[i] - '0') + carry;
                    if (product[k])
                        temp += product[k] - '0';
                    product[k++] = temp % 10 + '0';
                    carry = temp / 10;
                }
            if (carry > 0)
                product[k] = carry + '0';
            ls++;
        }
    reverseStr (product);
    bnStrip (product);
    return product;
}

char *
bigMul (char a[], char b[])
{
    uint16_t ta = strlen (a);
    uint16_t tb = strlen (b);
    if ((ta == 0) || (tb == 1 && *b == '1'))
        return a;
    else if ((tb == 0) || (ta == 1 && *a == '1'))
        return b;
    return mulSchool (a, b);
}

/* Duplicates a string into its own heap buffer. */
static char *
bnDup (const char *s)
{
    char *r = (char *)alloc (strlen (s) + 1, sizeof (char));
    strcpy (r, s);
    return r;
}

/* True if the decimal string equals zero (empty or all zeros). */
static bool
bnIsZero (const char *s)
{
    for (; *s; s++)
        if (*s != '0')
            return false;
    return true;
}

/* True if the decimal number is odd. */
static bool
bnIsOdd (const char *s)
{
    size_t n = strlen (s);
    return n && ((s[n - 1] - '0') & 1);
}

/* Divides the decimal string by 2, in-place (removes leading zeros). */
static void
bnHalve (char *s)
{
    int carry = 0;
    char *p;
    for (p = s; *p; p++)
        {
            int d = carry * 10 + (*p - '0');
            *p = (char)(d / 2 + '0');
            carry = d % 2;
        }
    /* remove leading zeros, keeping at least one digit */
    char *q = s;
    while (q[0] == '0' && q[1] != '\0')
        q++;
    if (q != s)
        memmove (s, q, strlen (q) + 1);
}

/* Product that ALWAYS returns its own buffer (bigMul may return an alias). */
static char *
bnMul (char *x, char *y)
{
    char *t = bigMul (x, y);
    if (t == x || t == y)
        return bnDup (t);
    return t;
}

/*
**  EXPONENTIATION BY SQUARING (binary): O(log exponent) multiplications,
**  instead of the original version's iterative O(exponent) multiplication.
**  Walks the decimal exponent dividing it by 2 at each step.
*/
char *
bigPow (char a[], char b[])
{
    if (*b == '0' || *b == '\0')
        {
            char *one = (char *)alloc (2, sizeof (char));
            one[0] = '1';
            one[1] = '\0';
            return one;
        }
    if (*b == '-')
        return NULL;

    char *result = (char *)alloc (2, sizeof (char));
    result[0] = '1';
    result[1] = '\0';
    char *base = bnDup (a);
    char *e = bnDup (b);

    while (!bnIsZero (e))
        {
            if (bnIsOdd (e))
                {
                    char *t = bnMul (result, base);
                    free (result);
                    result = t;
                }
            bnHalve (e);
            if (!bnIsZero (e))
                {
                    char *t = bnMul (base, base);
                    free (base);
                    base = t;
                }
        }
    free (base);
    free (e);
    return result;
}

/*
**  POSITIVE DIVISION/MODULO — classic long division.
**
**  Walks the dividend digit by digit, bringing each one into the current
**  remainder (cur = rem*10 + digit) and finding the quotient digit q in 0..9
**  by successive subtractions of D. Returns the quotient (DIV) or the
**  remainder (MOD), always in its own buffer. E.g.: 20/6 -> 3 (remainder 2);
**  3 % 10 -> 3.
*/
char *
bigDivMod (char a[], char D[], bool MOD)
{
    if (!*D)
        return bnDup ("E"); /* division by zero is undefined */
    int tn = strlen (a);
    char *rem = bnDup ("0"); /* current remainder */
    char *quot = (char *)alloc (tn + 1, sizeof (char));
    int qi = 0;
    for (int idx = 0; idx < tn; idx++)
        {
            /* bring in the next digit: cur = rem*10 + a[idx] */
            int rl = strlen (rem);
            char *cur = (char *)alloc (rl + 2, sizeof (char));
            strcpy (cur, rem);
            cur[rl] = a[idx];
            cur[rl + 1] = '\0';
            /* quotient digit: how many times D fits into cur (0..9) */
            int q = 0;
            while (numGreaterEqual (cur, D)) /* while cur >= D */
                {
                    char *t = bigSub (cur, D);
                    free (cur);
                    cur = t;
                    q++;
                }
            quot[qi++] = (char)(q + '0');
            free (rem);
            stripLeadingZeros (&cur); /* normalize the remainder */
            rem = cur;
        }
    quot[qi] = '\0';
    if (MOD)
        {
            free (quot);
            stripLeadingZeros (&rem);
            return rem; /* remainder ("" represents zero) */
        }
    free (rem);
    stripLeadingZeros (&quot);
    return quot; /* quotient ("" represents zero) */
}

bool
numGreaterEqual (char x[], char y[])
{
    char *a = x, *b = y;
    advancePastZeros (2, &a, &b);
    int ta = strlen (a), tb = strlen (b);
    if (ta > tb)
        return true;
    if (ta == tb)
        {
            int i;
            for (i = 0; a[i]; i++)
                {
                    if (a[i] > b[i])
                        return true;
                    else if (a[i] < b[i])
                        return false;
                }
        }
    else
        return false;
    return true;
}

char *
bigFactorial (char in[])
{
    char *inTemp = (char *)alloc (strlen (in) + 1, sizeof (char));
    register char *fat = (char *)alloc (_1KB, sizeof (char));
    strcpy (inTemp, in);
    int i = 0, k = 1, tamA = strlen (inTemp);
    if (tamA > 3)
        return NULL;
    i = 0;
    *fat = 1;
    long long num = 0ll;
    long long place = 1ll;
    reverseStr (inTemp);
    digitsToValues (inTemp);
    while (i < tamA)
        {
            num += (long long)inTemp[i] * place;
            place *= 10;
            i++;
        }
    free (inTemp);
    if (num > 400 || num < 0)
        return NULL;
    for (i = 2; i <= num; i++)
        k = factMulSmall (i, fat, k);
    valuesToDigits (fat, k + 1);
    reverseStr (fat);
    return fat;
}

int
factMulSmall (int a, char fat[], int limit)
{
    int count, product, resto = 0;
    for (count = 0; count < limit; count++)
        {
            product = fat[count] * a + resto;
            fat[count] = product % 10;
            resto = product / 10;
        }
    while (resto)
        {
            fat[limit] = resto % 10;
            resto = resto / 10;
            limit++;
        }
    return limit;
}

void
stripLeadingZeros (char **ptrNumber)
{
    char *number = *ptrNumber;
    int x;
    for (x = 0; *number == '0'; x++)
        number++;
    if (!x)
        return;
    int len = strlen (number);
    char *newNumber = (char *)alloc (len + 1, sizeof (char));
    strcpy (newNumber, number);
    free (number - x);
    *ptrNumber = newNumber;
}

void
advancePastZeros (int narg, ...)
{
    va_list argList;
    va_start (argList, narg);
    char **arg;
    while (narg--)
        {
            arg = va_arg (argList, char **);
            while (**arg == '0')
                (*arg)++;
        }
    va_end (argList);
}

/*
**  Applies f(a,b) and stores the result in a. Alias-safe: if f returns a
**  itself (shortcut), there is nothing to do; if it returns b, copies without
**  freeing b (the caller owns b). Only frees the result when it is a new
**  buffer.
*/
bool
applyInto (char a[], char b[], char *(*f) (char *, char *))
{
    char *temp = f (a, b);
    if (temp)
        {
            if (temp != a)
                strcpy (a, temp);
            if (temp != a && temp != b)
                free (temp);
        }
    else
        *a = '\0';
    return (a != NULL);
}

bool
applyDivInto (char a[], char b[], bool mod, char *(*f) (char *, char *, bool))
{
    char *temp = f (a, b, mod);
    if (temp)
        {
            if (temp != a)
                strcpy (a, temp);
            if (temp != a && temp != b)
                free (temp);
        }
    else
        *a = '\0';
    return (a != NULL);
}

void
swapChars (char *a, char *b)
{
    register char h = *a;
    *a = *b;
    *b = h;
}

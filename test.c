#include <stdio.h>
#include <string.h>

int main()
{
    char w[80] = "um dois tres";
    char *p = NULL;
    int i = 0;
    while (w[i]!='\0')
    {
        puts (&w[i]);
        p = strpbrk(&w[i], " ");
        if (! p) break;
        i=p+1-w;
    }

    return 0;
}
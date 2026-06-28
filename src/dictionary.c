#include "extenso/dictionary.h"
#include "extenso/util.h"

Dictionary *
dictionary_load (const char *path)
{
    FILE *f = openFile (path, "r");
    size_t cap = 64, n = 0;
    DictEntry *items = (DictEntry *)alloc (cap, sizeof (DictEntry));
    char line[MAX_GEN];
    while (fgets (line, sizeof line, f))
        {
            line[strcspn (line, "\r\n")] = '\0';
            if (!line[0])
                continue; /* ignore blank lines */
            char *eq = strchr (line, '=');
            if (!eq)
                continue; /* a line without '=' is not an entry */
            *eq = '\0';
            if (n == cap)
                {
                    cap <<= 1;
                    items = (DictEntry *)realloc (items, cap * sizeof (DictEntry));
                    if (!items)
                        abortWithLog (true);
                }
            strcpy (items[n].name, line);
            strcpy (items[n].value, eq + 1);
            n++;
        }
    fclose (f);

    Dictionary *d = (Dictionary *)alloc (1, sizeof (Dictionary));
    d->items = items;
    d->n = n;
    /* delim_start: first entry whose value does not start with a digit. */
    d->delim_start = n;
    for (size_t i = 0; i < n; i++)
        if (!isdigit ((unsigned char)items[i].value[0]))
            {
                d->delim_start = i;
                break;
            }
    return d;
}

void
dictionary_free (Dictionary *d)
{
    if (!d)
        return;
    free (d->items);
    free (d);
}

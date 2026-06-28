#include "extenso/lexer.h"
#include "extenso/errors.h"
#include "extenso/num_list.h"
#include "extenso/tokens.h"

/* Advances the cursor past the matched word (letters and spaces, plus hyphens
** for compound delimiters) and restores the character saved in `trade`. */
static void
advancePastMatch (Context *ctx, char trade, bool hyphen)
{
    while (*ctx->cursor && (isalpha (*ctx->cursor) || *ctx->cursor == ' ' || (hyphen && *ctx->cursor == '-')))
        ctx->cursor++;
    *ctx->cursor = trade;
}

typedef enum
{
    SCAN_CONTINUE, /* this entry did not produce a token; keep scanning */
    SCAN_RESTART,  /* token consumed; rescan the dictionary for the next word */
    SCAN_DONE      /* a delimiter / end was reached; nextToken should return */
} ScanResult;

/* Handles a dictionary entry whose name matched the current word: emits either
** a number token or a delimiter/conjunction token. */
static ScanResult
handleMatch (Context *ctx, int i, Term *ref, char trade)
{
    char value = ref->value[0];
    if (isdigit (value))
        {
            ctx->token = value;
            advancePastMatch (ctx, trade, false);
            ctx->tokenType = NUM;
            ctx->isNumber = true;
            listAppend (ctx, i, ref->name, ref->value);
            return nextIsDelimiter (ctx) ? SCAN_DONE : SCAN_RESTART;
        }
    if (!strchr (STR_DEL, value))
        return SCAN_CONTINUE;

    ctx->tokenType = CONJUCAO;
    advancePastMatch (ctx, trade, true);
    ctx->token = value;
    if (i != CONJUCAO)
        {
            ctx->tokenType = DELIMITADOR;
            ctx->isNumber = false;
            return SCAN_DONE;
        }
    listAppend (ctx, i, ref->name, ref->value); /* the conjunction 'e' is enqueued */
    return SCAN_RESTART;
}

void
nextToken (Context *ctx)
{
    Term ref;
    int i = 0, k = 0;
    char trade;
    ctx->token = '\0';
    ctx->tokenType = 0;
    if (!*ctx->cursor)
        return;
    while (isspace (*ctx->cursor))
        ++ctx->cursor;
    while (ctx->cursor[k] && isalpha (ctx->cursor[k]))
        k++;
    trade = ctx->cursor[k];
    ctx->cursor[k] = '\0';
    joinCompoundDelim (ctx, &k, &trade);
    while (i < (int)ctx->dict->n)
        {
            strcpy (ref.name, ctx->dict->items[i].name);
            strcpy (ref.value, ctx->dict->items[i].value);
            if (strcmp (ref.name, ctx->cursor) && !matchPlural (ctx, i, ref.name))
                {
                    skipWord (ctx);
                    i++;
                    continue;
                }
            ScanResult r = handleMatch (ctx, i, &ref, trade);
            if (r == SCAN_DONE)
                return;
            if (r == SCAN_RESTART)
                i = -1; /* i++ below restarts the scan from 0 */
            i++;
        }
    reportError (ctx, 0);
}

void
skipWord (Context *ctx)
{
    while (*ctx->cursor && *ctx->cursor == ' ')
        ctx->cursor++;
    int k = strcspn (ctx->cursor, (const char *)" ");
    ctx->cursor[k] = '\0';
}

bool
nextIsDelimiter (Context *ctx)
{
    while (*ctx->cursor && *ctx->cursor == ' ')
        ctx->cursor++; /* position at the start of the next token */
    char *space = strchr (ctx->cursor, ' ');
    if (!space)
        return *ctx->cursor ? 0 : 1;
    int k = space - ctx->cursor;
    ctx->cursor[k] = '\0';

    char del[MAXWLEN] = { '\0' };
    bool found = false;
    /* delim_start + 1 skips the 'e' (first delimiter). */
    for (size_t j = ctx->dict->delim_start + 1; j < ctx->dict->n && !found; j++)
        {
            strcpy (del, ctx->dict->items[j].name);
            char *hyphen = strchr (del, '-'); /* drop the compound-delimiter tail */
            if (hyphen)
                *hyphen = '\0';
            found = !strcmp (del, ctx->cursor);
        }
    ctx->cursor[k] = ' ';
    return found;
}

bool
matchPlural (Context *ctx, int i, char *currentToken)
{
    (void)i;
    if (!strchr ("mbtqdscount", *currentToken))
        return false;
    char *comma = strchr (currentToken, ',');
    if (!comma)
        return false;
    int k = comma - currentToken;
    currentToken[k] = '\0';
    if (!strcmp (currentToken, ctx->cursor)) /* singular form matched */
        return true;
    char *plural = comma + 1;
    if (strcmp (plural, ctx->cursor)) /* neither form matched */
        return false;
    /* plural form matched: keep it as the token's name */
    char temp[MAXWLEN] = { '\0' };
    strcpy (temp, plural);
    strcpy (currentToken, temp);
    currentToken[++k] = '\0';
    return true;
}

/* A word that, joined with the next, forms a compound delimiter. */
static bool
isCompoundHead (const char *word)
{
    return !strcmp (word, "abre") || !strcmp (word, "fecha") || !strcmp (word, "dividido") || !strcmp (word, "fatorial") || !strcmp (word, "elevado");
}

/* The second half of a compound delimiter. */
static bool
isCompoundTail (const char *word)
{
    return !strcmp (word, "parentese") || !strcmp (word, "de") || !strcmp (word, "por") || !strcmp (word, "a");
}

void
joinCompoundDelim (Context *ctx, int *k, char *temp)
{
    char head = *ctx->cursor;
    if (head != 'a' && head != 'f' && head != 'd' && head != 'e')
        return;
    if (!isCompoundHead (ctx->cursor))
        return;

    int i = 0;
    ctx->cursor[*k] = '-';
    while (isalpha (ctx->cursor[i]) || ctx->cursor[i] == '-')
        i++;
    *temp = ctx->cursor[i];
    ctx->cursor[i] = '\0';
    if (isCompoundTail (&ctx->cursor[*k + 1]))
        return;

    ctx->cursor[*k] = ' ';
    reportError (ctx, 0);
}

void
lowercaseExpr (Context *ctx)
{
    int i = 0;
    while (ctx->cursor[i])
        {
            ctx->cursor[i] = tolower (ctx->cursor[i]);
            i++;
        }
}

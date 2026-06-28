#ifndef EXTENSO_CONVERSION_H
#define EXTENSO_CONVERSION_H

/*
**  Conversion between spelled-out form (extenso) and a digit string.
*/
#include "extenso/config.h"
#include "extenso/context.h"

/* Converts from spelled-out form (in the list) to a digit string. */
char *toDigits (Context *ctx);

/* Converts from a digit string to spelled-out form. */
void toWords (Context *ctx, char **answer);

/* Converts the hundreds/tens/units to spelled-out form. */
int toWordsTriplet (Context *ctx, char **str, char *result, uint16_t *size, uint16_t *flagPlural);

#endif

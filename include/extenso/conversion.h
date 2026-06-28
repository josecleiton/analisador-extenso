#ifndef EXTENSO_CONVERSION_H
#define EXTENSO_CONVERSION_H

/*
**  Conversão extenso <-> string de dígitos.
*/
#include "extenso/config.h"
#include "extenso/context.h"

/* Converte de extenso (na lista) para uma string de dígitos. */
char *toDigits (Context *ctx);

/* Converte de uma string de dígitos para extenso. */
void toWords (Context *ctx, char **answer);

/* Converte a centena/dezena/unidade para extenso. */
int toWordsTriplet (Context *ctx, char **str, char *result, uint16_t *size, uint16_t *flagPlural);

#endif

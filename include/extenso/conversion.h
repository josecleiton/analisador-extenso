#ifndef EXTENSO_CONVERSION_H
#define EXTENSO_CONVERSION_H

/*
**  Conversão extenso <-> string de dígitos.
*/
#include "extenso/config.h"
#include "extenso/context.h"

/* Converte de extenso (na lista) para uma string de dígitos. */
char *toNum (Context *ctx);

/* Converte de uma string de dígitos para extenso. */
void toName (Context *ctx, char **resposta);

/* Converte a centena/dezena/unidade para extenso. */
int toNameMenOrd (Context *ctx, char **str, char *resultado, uint16_t *size, uint16_t *flagPlural);

#endif

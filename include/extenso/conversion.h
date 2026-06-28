#ifndef EXTENSO_CONVERSION_H
#define EXTENSO_CONVERSION_H

/*
**  Conversão extenso <-> string de dígitos.
*/
#include "extenso/config.h"

/* Converte de extenso (na lista) para uma string de dígitos. */
char *toNum (void);

/* Converte de uma string de dígitos para extenso. */
void toName (char **resposta);

/* Converte a centena/dezena/unidade para extenso. */
int toNameMenOrd (char **str, char *resultado, uint16_t *size, uint16_t *flagPlural);

#endif

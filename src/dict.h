/*
 * =====================================================================================
 *
 *       Filename:  dict.h
 *
 *    Description: dict with bucket hash
 *
 *        Version:  1.0
 *        Created:  12/15/2019 11:21:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef DICT
#define DICT

#include "hash.h"

/*
**  DICIONÁRIO
**  <chave>=<valor>
**
**  chave é o número por extenso aceito
**  valor é o número cardinal em linguagem matemática
**  Exemplo: trezentos=300
*/
#define ARQ_DICT "lib/dicionario.cfg"

/*
**  MAXWLEN
**  tamanho maximo das palavras no ARQ_DICT (antes e depois do '=')
*/
#define MAXWLEN 35

struct dict {
   const char key[MAXWLEN];
   const char value[MAXWLEN];
};

typedef struct classe_value {
   char valor[MAXWLEN];
   short classe;
} BucketValueDS;

extern const struct dict dicionario[];
extern BucketHash* dict;

/*
 * INIT DAT
 */
BucketHash* initDict(void);

#endif

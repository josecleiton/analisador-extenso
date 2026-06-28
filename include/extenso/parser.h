#ifndef EXTENSO_PARSER_H
#define EXTENSO_PARSER_H

/*
**  Avaliador por descida recursiva. Dirige o lexer, chama as operações de
**  bignum e devolve o resultado como string de dígitos.
*/

/* Gatilho de partida: avalia EXP e devolve o resultado por extenso. */
char *expParsingStart (void);

/* Soma/subtrai termos. */
void expResTermo (char *resposta);

/* Multiplica/divide/mod/eleva fatores. */
void expResFator (char *resposta);

/* Resolve o fatorial de um fator. */
void expResFatorial (char *resposta);

/* Resolve uma expressão entre parênteses. */
void expResParenteses (char *resposta);

/* Devolve o valor numérico das expressões por extenso. */
void atomo (char *resposta);

#endif

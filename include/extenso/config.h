#ifndef EXTENSO_CONFIG_H
#define EXTENSO_CONFIG_H

/*
**  Includes de sistema e macros de configuração compartilhados.
*/
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Arquivos de dados (relativos ao diretório de trabalho). */
#define ARQ_DICT "lib/dicionario.cfg"    /* <palavra>=<value> por linha */
#define ARQ_ERROS "lib/erros.cfg"        /* um tipo de erro por linha */
#define ARQ_ENTRADA "lib/expressoes.txt" /* expressões a analisar */
#define ARQ_SAIDA "resultados.txt"       /* result das expressões */
#define ARQ_LOG "logs.txt"               /* data + descrição de cada erro */

/* STR_DEL: caracteres delimitadores. */
#define STR_DEL "+/%-*!e()^"

/* MAX_GEN: tamanho genérico de string. */
#define MAX_GEN 1024

/* MAXWLEN: tamanho máximo das palavras no ARQ_DICT (antes e depois do '='). */
#define MAXWLEN 35

/* _1KB: 2^10 bytes. */
#define _1KB 1024

/* CLRBUF: limpeza do buffer de entrada. */
#define CLRBUF scanf ("%*c")

#endif

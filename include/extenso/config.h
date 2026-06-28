#ifndef EXTENSO_CONFIG_H
#define EXTENSO_CONFIG_H

/*
**  Includes de sistema e macros de configuração compartilhados.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

/* Arquivos de dados (relativos ao diretório de trabalho). */
#define ARQ_DICT    "lib/dicionario.cfg"  /* <palavra>=<valor> por linha */
#define ARQ_ERROS   "lib/erros.cfg"       /* um tipo de erro por linha */
#define ARQ_ENTRADA "lib/expressoes.txt"  /* expressões a analisar */
#define ARQ_SAIDA   "resultados.txt"      /* resultado das expressões */
#define ARQ_LOG     "logs.txt"            /* data + descrição de cada erro */

/* TAM_DICT/INDEL/NUM_ERROS: contagens acopladas aos .cfg (removidos na Fase 3). */
#define TAM_DICT  58   /* número de linhas do ARQ_DICT */
#define INDEL     48   /* linha do ARQ_DICT onde começam os delimitadores */
#define NUM_ERROS 13   /* número de linhas do ARQ_ERROS */

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

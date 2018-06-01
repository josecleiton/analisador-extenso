# Analisador de expressões numéricas por extenso em C

## Introdução
Este é um **analisador de expressões númericas por extenso**, que resolve expressões escritas em português, seguindo algumas regras bem definidas.
A linguagem de programação é C e funciona bem com o GCC 8.1.0.

## Da motivação
Este projeto foi proposto como "trabalho de semestre" da disciplina Linguagem de Programação I, pelo professor [Jorge Sampaio Farias](http://lattes.cnpq.br/6683499592786376), referente ao período 2 do curso de [Sistemas de Informação](http://www.csi.uneb.br/) na [Universidade do Estado da Bahia (UNEB)](https://portal.uneb.br/).

##  Analisador de expressões?

##  Analisador léxico?

##  Analisador sintático?

##  Analisador semântico?

## Operações com números em strings

## Instruções de uso
**Não utilize acentos nas palavras.**
- Correto = dois milhoes
- Incorreto = ~~dois milhões~~

**Não utilize pontuação**
- Correto = sete trilhoes e oitenta milhoes
- Incorreto = ~~sete trilhoes, e oitenta milhoes~~

## Adição de mais ordens no dicionario
Após adicionar a referida ordem, cuide com o __size__ da macro MALLOC (em diversos ponteiros no arquivo **main.c**).
Sugiro que altere as alocações dos seguintes ponteiros
-
-
-
-
-

## Tratamento dos arquivos erros.cfg e dicionario.cfg
Por motivo de tentar manter o projeto portável, foi assumida a padronização LF (GNU/Linux) para os arquivos e não CRLF (Windows) ou CR (macOS), porque eu, o desenvolvi todo em uma distribuição GNU/Linux. Portanto, se você decidir alterar qualquer um desses arquivos, sugiro a utilização do Notepad++ para a conversão deles de volta para LF, senão o comportamento da saída (resultado das expressões) é indefinido.

## Saida
O arquivo de saida __resultados.txt__ também está em LF, então sugiro a visualização por um editor de texto que trate disso (desconsidere se você é um usuário GNU/Linux).

## Descrição das funções

##

## Bibliografia
Todos os documentos de apoio podem ser encontrados na pasta **Documentos**. Agradeço a todos os autores e os credito pela contribuição ao projeto e à minha formação.


## Contato
A partir do email do Github (__jcleitonbc@gmail.com__).
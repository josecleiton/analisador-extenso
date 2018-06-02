# Analisador de expressões numéricas por extenso em C

## Introdução

Este é um **analisador de expressões númericas por __extenso__**, que resolve expressões escritas em português, seguindo algumas regras bem definidas.
A linguagem de programação utilizada: ISO C90 ou **C**.

## Da motivação

Este projeto foi proposto como "trabalho de semestre" da disciplina Linguagem de Programação I, pelo professor [**Jorge Sampaio Farias**](http://lattes.cnpq.br/6683499592786376), referente ao período 2 do curso de [Sistemas de Informação](http://www.csi.uneb.br/) na [**Universidade do Estado da Bahia (UNEB)**](https://portal.uneb.br/).

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

**Da adição de ordens no dicionário**

Sugiro que antes da edição do arquivo de dicionário, leia a próxima seção. Se o dicionário foi aumentado, cuide com a alocação de alguns ponteiros, o tamanho de algumas variáveis e definição de constantes, no arquivo principal *main.c*. No caso dos ponteiros, acrescente adequadamente o parâmetro *size* da macro **MALLOC**. Alguns pontos para tomar cuidado:

- Variável global *expNum*
- Alocações
    - *resposta*
    - *ref*
    - *segFator*
    - *segTermo*
    - *proxFator*
    - *strErro*
- Constante *TAM*
- Comparação lógica *filaCount() > 43* (limite padrão = decilhões)

**Da adição de erros**

Atente para a constante *NUM_ERROS*, que numera a quantidade de linhas do arquivo **erros.cfg**, cada linha com um erro diferente. É bom ter cuidado com a alocação do ponteiro *strErro*, o qual guarda a string formatada com o erro e um cursor que indica a ocorrencia do erro. Novamente, é só adequar o parâmetro *size* da macro 


## Tratamento dos arquivos *erros.cfg* e *dicionario.cfg*

Por motivo de tentar manter o projeto portável, foi assumida a padronização LF (GNU/Linux) para os arquivos e não CRLF (Windows) ou CR (macOS), porque eu, o desenvolvi todo em uma distribuição **GNU/Linux**. Portanto, se você decidir alterar qualquer um desses arquivos, sugiro a utilização do Notepad++ para a conversão deles de volta para LF, senão o comportamento da saída (resultado das expressões) é indefinido.

## Saida

O arquivo de saida *resultados.txt* também está em LF, então sugiro a visualização por um editor de texto que trate disso (desconsidere se você é um usuário GNU/Linux).

## Descrição das funções

##

## Bibliografia

Todos os documentos de apoio podem ser encontrados na pasta **Documentos**. Agradeço a todos os autores e os credito pela contribuição ao projeto e à minha formação.


## Contato

A partir do [email](mailto:jcleitonbc@gmail.com) do Github.
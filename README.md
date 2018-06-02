# Analisador de expressões numéricas por extenso em C

## Introdução

Este é um **analisador de expressões númericas por __extenso__**, que resolve expressões escritas em português, seguindo algumas regras bem definidas.
A linguagem de programação utilizada: ISO C90 ou apenas **C**.
## Da motivação

Este projeto foi proposto como trabalho final da disciplina Linguagem de Programação I, pelo professor [**Jorge Sampaio Farias**](http://lattes.cnpq.br/6683499592786376), referente ao período 2 do curso de [Sistemas de Informação](http://www.csi.uneb.br/) na [**Universidade do Estado da Bahia (UNEB)**](https://portal.uneb.br/).

## Do limite

O limite de expressões atualmente está em 0 ≤ N < 10³⁴. Mas, o projeto continuará e o próximo passo é atingir 1/10³⁴ < N < 10³⁴.

## Conceitos

###  Analisador de expressões?

###  Analisador léxico?

###  Analisador sintático?

###  Analisador semântico?

### Operações com números em strings

Em vista do N ser muito maior do que um long long int, foi necessário colocar os números como strings, cada digito destes representa uma posição no vetor de caracteres.

## Instruções de uso

### Compilação

Por ser uma aplicação em C, você necessita do GCC na sua máquina. Algumas IDEs instalam-no em conjunto, como o DEV C++, mas o seguinte tutorial é para compilá-la no terminal.

- Clone o repositório
- Abra a pasta *compilha* no seu terminal
- Verifique se o GCC está instalado com `gcc --version`
- Após isso compile com o seguinte código: `gcc main.c -o main -lm`
- Se tiver utilizando Windows, altere o main para main.exe

### Do programa

As expressões devem ser formatadas com espaços delimitando as palavras.

#### Formato das operações
- Soma 🢥 numero *mais* numero
- Subtração 🢥 numero *menos* numero
- Multiplicação 🢥 numero *vezes* numero
- Divisão 🢥 numero *dividido por* numero
- Resto da divisão 🢥 numero *mod* numero
- Fatorial 🢥 *fatorial de* numero

##### OBS: deve-se seguir estritamente as instruções para a analise ocorrer como esperado.

#### Não utilize acentos nas palavras
- Correto = dois milhoes
- Incorreto = ~~dois milhões~~

#### Não utilize pontuação
- Correto = sete trilhoes e oitenta milhoes
- Incorreto = ~~sete trilhoes, e oitenta milhoes~~

### Da adição de ordens no dicionário

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

### Da adição de erros

Atente para a constante *NUM_ERROS*, que numera a quantidade de linhas do arquivo **erros.cfg**, cada linha com um erro diferente. É bom ter cuidado com a alocação do ponteiro *strErro*, o qual guarda a string formatada com o erro e um cursor que indica a ocorrencia do erro. Novamente, é só adequar o parâmetro *size* da macro 


## Tratamento dos arquivos *erros.cfg* e *dicionario.cfg*

Foi assumida a padronização LF (GNU/Linux) para os arquivos e não CRLF (Windows) ou CR (macOS), porque eu, o desenvolvi em uma distribuição **GNU/Linux**. Portanto, se você utilizar um desses SO proprietários e decidir alterar qualquer um desses arquivos, sugiro a utilização do **Notepad++** para a conversão deles de volta para LF, senão o comportamento da saída (resultado das expressões) é indefinido.

## Saída

O arquivo de saida *resultados.txt* também está em LF, então sugiro a visualização por um editor de texto que trate disso (desconsidere se você é um usuário GNU/Linux).

## Descrição das funções

O funcionamento das funções (e seu algoritmo) estão descritos no arquivo de cabeçalho *lib/main.h*. As funções de operações básicas são "algoritmos clássicos" de soma, subtração, divisão e multiplicação, digito por digito, como aprendido no Ensino Fundamental do Brasil.


## Bibliografia

Todos os documentos de apoio podem ser encontrados na pasta [ExpressionParsing](https://www.dropbox.com/sh/r32lw4bn3owka2m/AAApL63R1qBi4EdnlrZKaBqMa?dl=0) no Dropbox. Agradeço a todos os autores e os credito pela contribuição ao projeto e à minha formação.


## Contato

A partir do [email](mailto:jcleitonbc@gmail.com) do Github.
# Analisador de expressões numéricas por extenso em C

## Introdução

Este é um **analisador de expressões númericas por __extenso__**, em português, seguindo algumas regras bem definidas.
A linguagem de programação utilizada: ISO C90 ou apenas **C**.
## Da motivação

Este projeto foi proposto como trabalho final da disciplina Linguagem de Programação I, pelo professor [**Jorge Sampaio Farias**](http://lattes.cnpq.br/6683499592786376), referente ao período 2 do curso de [Sistemas de Informação](http://www.csi.uneb.br/) na [**Universidade do Estado da Bahia (UNEB)**](https://portal.uneb.br/).

## Do limite

O limite de expressões atualmente está em 0 ≤ N < 10³⁴. Mas, o projeto continuará e o próximo passo é atingir 1/10³⁴ < N < 10³⁴.

## Conceitos

###  Interpretador de expressões

A proposta é de resolver expressões númericas por extenso usando a linguagem C, um problema que requer uma interpretação das expressões de entrada. Porque mesmo que resolvamos expressões simples facilmente, isto é, com apenas uma operação (é necessário atentar para o N), quando há várias operações, fica muito difícil programar o computador, sem deixar o código ilegível. A partir dessa premissa e com os materiais de apoio, implementei um conjunto de rotinas que resolvem corretamente cada operação no momento desejado. Esse subconjunto de rotinas têm prefixo **exp**.

Além disso, precisamos de um analisador gramatical (ou léxico) para sabermos se as palavras que compõem a expressão são válidas, este analisador quebra a expressão em vários tokens. Isso é o que a função `pega_token` faz.

Um outro subconjunto de rotinas é necessário para a verificação semântica, ou seja, de significado das palavras ali colocadas. Porque "milhao um" está lexicamente correto, porém, não tem o mesmo significado que "um milhao". Nós, nativos da lingua, fazemos essa avaliação automaticamente, mas foi necessário passar isso para o computador. Esse subconjunto de rotinas tem prefixo **sem**.

 ![Formalismo](Documentos/analisador.jpg?raw=true "Formalismo de Backus-Naur")
 ###### Gramática de Backus-Naur utilizada pelo analisador.

### Operações com números em strings

Em vista do N ser muito maior do que um `long long`, foi necessário guardar os números como strings, cada digito destes representa uma posição no vetor de caracteres.

## Instruções de uso

### Compilação

Por ser uma aplicação em C, você necessita de um compilador C na sua máquina, usarei o GCC como exemplo abaixo. Algumas IDEs instalam-no em conjunto, como o DEV C++, mas o seguinte tutorial é para compilá-la no terminal.

- Clone o repositório
- Abra a pasta *compilha* no seu terminal
- Verifique se o GCC ou o TCC está instalado com `gcc --version` ou `clang --version`
- Se não estiver instalado, use o gerenciador de pacotes da sua distribuição para resolver isso.
- Após isso compile com o seguinte código: `clang -O2 *.c -o main -lm` ou `gcc -O2 *.c -o main -lm` 
- Se estiver utilizando Windows, altere o `main` para `main.exe`

### Do programa

As expressões devem ser formatadas com espaços delimitando as palavras.

#### Formato das operações
- Soma 🢥 numero *mais* numero
- Subtração 🢥 numero *menos* numero
- Multiplicação 🢥 numero *vezes* numero
- Divisão 🢥 numero *dividido por* numero
- Resto da divisão 🢥 numero *mod* numero
- Fatorial 🢥 *fatorial de* numero
- Potenciação 🢥 numero *elevado a* numero __*NEW!*__

##### OBS: deve-se seguir estritamente as instruções para a analise ocorrer como esperado.

#### Não utilize acentos nas palavras
- Correto = dois milhoes
- Incorreto = ~~dois milhões~~

#### Não utilize pontuação
- Correto = sete trilhoes e oitenta milhoes
- Incorreto = ~~sete trilhoes, e oitenta milhoes~~

## Descrição das funções

Os comportamentos das funções (e seu algoritmo) estão descritos no arquivo de cabeçalho `interpretador.h`. As funções de operações básicas são "algoritmos clássicos" de soma, subtração, divisão e multiplicação.


## Bibliografia

Todos os documentos de apoio podem ser encontrados na pasta [ExpressionParsing](https://www.dropbox.com/sh/r32lw4bn3owka2m/AAApL63R1qBi4EdnlrZKaBqMa?dl=0) no Dropbox. Agradeço a todos os autores e os credito pela contribuição ao projeto e à minha formação.


## Contato

A partir do [email](mailto:jcleitonbc@gmail.com) do Github.

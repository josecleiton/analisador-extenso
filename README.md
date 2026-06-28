# Spelled-out numeric expression analyzer in C

## Introduction

This is an **analyzer of numeric expressions written out in full (`por extenso`)**, in
Portuguese, following a few well-defined rules. The implementation language is ISO **C11**
(the code uses `stdbool.h`, `stdint.h`, mid-block declarations, etc.).

The input numbers are written the way they are spoken in Portuguese (e.g. `dois milhoes`,
`setenta e quatro`), so the domain vocabulary in the dictionary, the input/output and the
on-screen messages are intentionally kept in Portuguese.

## Motivation

This project was proposed as the final assignment for the course *Linguagem de Programação I*,
by professor [**Jorge Sampaio Farias**](http://lattes.cnpq.br/6683499592786376), in the 2nd
term of the [Information Systems](http://www.csi.uneb.br/) program at the
[**State University of Bahia (UNEB)**](https://portal.uneb.br/).

## Concepts

### Expression interpreter

The goal is to evaluate spelled-out numeric expressions in C — a problem that requires
interpreting the input. A single-operation expression is easy, but with several operations it
gets hard to program without making the code unreadable. The evaluator is a recursive-descent
parser (`parseTerm` / `parseFactor` / `parseFactorial` / `parseParen` / `parseAtom` in
`src/parser.c`) that resolves each operation at the right moment.

A lexical analyzer is also needed to check whether the words that make up an expression are
valid; it breaks the expression into tokens. That is what `nextToken` (in `src/lexer.c`) does.

A third group of routines performs **semantic** validation — the meaning of the words. For
example, `milhao um` is lexically correct but does not mean the same as `um milhao`. Native
speakers do this automatically; here it had to be taught to the computer. See
`src/semantics.c`.

 ![Formalism](Documentos/analisador.jpg?raw=true "Backus–Naur formalism")
 ###### Backus–Naur grammar used by the analyzer.

### Arithmetic on numbers stored as strings

Because the operands can be far larger than a `long long`, the numbers are stored as strings
(arbitrary precision), where each digit is one position in the character array. The bignum
routines live in `src/bignum.c` (addition, subtraction, schoolbook multiplication, long
division/modulo, exponentiation by squaring and factorial).

## Project layout

```
include/extenso/   public headers, one per module
src/               cli, lexer, parser, semantics, conversion, num_list,
                   dictionary, errors, bignum, util, main
lib/               data files (Portuguese vocabulary): dicionario.cfg,
                   erros.cfg, expressoes.txt
tests/             golden + correctness + error-recovery test harness
```

## Usage

### Building

You need a C compiler. With the provided Makefile:

- Build: `make` (produces `build/analisador`).
- Debug build: `make debug` (produces `build/analisador-debug`, `-g -O0`).
- Run the tests: `make test`.
- Format the code consistently: `make format` (requires `clang-format`; check with
  `make format-check`).

Without the Makefile: `cc -std=c11 -O2 -Iinclude src/*.c -o analisador`.

### Running

- Interactive menu: `./build/analisador` (choose keyboard or file input).
- Evaluate one expression: `./build/analisador --eval "setenta e quatro mais dez"`.
- Batch mode: `./build/analisador --batch [input.txt] [output.txt]` (defaults to
  `lib/expressoes.txt` → `resultados.txt`).

Expressions must be written with spaces separating the words.

#### Operation formats
- Addition → numero *mais* numero
- Subtraction → numero *menos* numero
- Multiplication → numero *vezes* numero
- Division → numero *dividido por* numero
- Remainder → numero *mod* numero
- Factorial → *fatorial de* numero
- Exponentiation → numero *elevado a* numero

##### NOTE: follow the rules strictly for the analysis to work as expected.

#### Do not use accents
- Correct = `dois milhoes`
- Wrong = ~~`dois milhões`~~

#### Do not use punctuation
- Correct = `sete trilhoes e oitenta milhoes`
- Wrong = ~~`sete trilhoes, e oitenta milhoes`~~

## Function documentation

Each module's behavior is documented in its public header under `include/extenso/`. The basic
arithmetic routines in `bignum` are the classic algorithms for addition, subtraction,
multiplication and long division.

## Bibliography

The supporting material can be found in the
[ExpressionParsing](https://www.dropbox.com/sh/r32lw4bn3owka2m/AAApL63R1qBi4EdnlrZKaBqMa?dl=0)
Dropbox folder. Thanks to all the authors for their contribution to this project and to my
education.

## Contact

Through the [email](mailto:jcleitonbc@gmail.com) on GitHub.

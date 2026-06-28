# CLAUDE.md — analisador-extenso

Guidance for working in this repository.

## What this is

A CLI in C that evaluates **numeric expressions written out in full in Portuguese**
("por extenso"), e.g. `setenta e quatro mais dez` → `oitenta e quatro`. It supports
`+ - * / mod`, factorial, exponentiation and parentheses, over **arbitrary-precision**
integers (bignums stored as digit strings). Input vocabulary, on-screen messages and the
`.cfg`/`.txt` data files are in **Portuguese** (that's the language being parsed); code
identifiers and comments are in **English**.

## Layout

```
include/extenso/   public header per module (config, context, tokens, types,
                   dictionary, errors, lexer, parser, semantics, conversion,
                   num_list, bignum, util, cli)
src/               one .c per module + main.c
lib/               data (pt): dicionario.cfg, erros.cfg, expressoes.txt
tests/             run_golden.sh, run_cases.sh, run_errors.sh, run_memcheck.sh
                   + fixtures (resultados.golden.txt, cases.txt, expr_errors.txt)
```

Pipeline: `cli` → `lexer` (`nextToken`) → `parser` (recursive descent
`parseTerm/Factor/Factorial/Paren/Atom`) → `semantics` (validate) → `conversion`
(`toDigits`/`toWords`) → `bignum` (arithmetic on digit strings).

## Build / run / test

```sh
make                 # release → build/analisador
make debug           # → build/analisador-debug (-g -O0)
make test            # golden + correctness cases + error recovery
make memcheck        # valgrind (Linux only; see below)
make format          # clang-format -i (requires clang-format)
make format-check    # clang-format dry-run

./build/analisador                       # interactive menu
./build/analisador --eval "<expr>"       # evaluate one expression
./build/analisador --batch [in] [out]    # batch (default lib/expressoes.txt → resultados.txt)
```

## Conventions

- **Standard:** ISO C11. Build is warning-clean under `-Wall -Wextra -Wpedantic`; keep it so.
- **Formatting:** `.clang-format` (GNU base, 4-space indent, no line reflow). Run `make format`
  before committing; CI runs the build/tests, not the formatter.
- **Naming:** English identifiers. Keep Portuguese only where it is the analyzed domain: the
  `tokens` enum values (ZERO, UM, ...), the `.cfg` words, and user-facing strings/messages.
- **No global state:** everything per-analysis lives in `Context` (`include/extenso/context.h`),
  threaded explicitly. Don't reintroduce globals.
- **Data loaded once:** the dictionary and error table are read into memory at startup
  (`dictionary_load`, `error_table_load`) — never re-read files per token/error.
- **Error handling:** user-input errors call `reportError` which `longjmp`s back to the
  per-expression handler in `cli`/`main` (so one bad expression doesn't kill the program).
  `abort` (via `abortWithLog`) is reserved for unrecoverable infra failures (OOM, missing file).
- **Memory:** per-expression scratch buffers come from the `Context` arena (`arenaAlloc`),
  freed in bulk by `arenaReset` at the start of each expression and `arenaFree` at shutdown —
  this is what makes the `longjmp` path leak-free. If you add per-expression scratch, use
  `arenaAlloc` (do not `free` it individually). Transient bignum results are returned by the
  operation and freed by `applyInto`/`applyDivInto`.
- **Bignums:** numbers are digit strings (`'0'..'9'`, most-significant first; `""` means 0).
  Algorithms: schoolbook add/sub, single-level (non-recursive) Karatsuba multiply
  (`KARATSUBA_CUT` in `bignum.c`), long division/modulo, exponentiation by squaring, factorial.
  No `math.h`/floating point — integer arithmetic only.
- **Style preferences (this codebase):** prefer early returns; avoid `else` after a `return`;
  keep functions small — extract intermediate helpers for readability. Keep changes
  performance-neutral.

## Testing routine (run after every change)

1. `make test` — must print `GOLDEN OK`, `CASES OK`, `ERRORS OK`.
2. `make memcheck` — must print `MEMCHECK OK`. **Always run this after touching allocation,
   ownership, the arena, or the bignum/parse paths.** On macOS/Apple Silicon valgrind is
   unavailable, so run it in a Linux container:
   ```sh
   docker run --rm -v "$PWD":/src -w /src ubuntu:24.04 bash -c \
     'apt-get update -qq && apt-get install -y -qq build-essential valgrind && make && make memcheck'
   ```
3. For multiplication changes, also brute-force `bigMul` against Python (CUT lowered to force the
   Karatsuba path) — see commit history of `src/bignum.c`.

Two golden baselines exist: `tests/resultados.golden.txt` (current contract, clean spacing) and
`tests/resultados.frozen.txt` (historical, pre-spacing-fix). The harness compares against the
former by default. Test results in `tests/cases.txt` are verified against real arithmetic
(Python) — divisions/mod were 10× wrong / empty before the bignum fixes, so do not "restore"
those.

## Gotchas

- **`build/analisador` may become a Linux ELF** after running valgrind in Docker (the volume is
  shared). If you hit `exec format error` on macOS, run `make clean && make`.
- **Never let a script rewrite `src/bignum.c` in place** (a benchmark once truncated it). Operate
  on copies for experiments.
- **ASan/UBSan hang** in this macOS/clang environment even on trivial input, so `make debug` is
  `-g -O0` (no sanitizers). Use valgrind (in Linux/Docker) for memory checks.
- The interactive menu consumes one leading stdin char (`CLRBUF`) by design; tests drive the
  program through `--batch`/`--eval`, not scripted menu keystrokes.

## Adding things

- **New number word:** add a line to `lib/dicionario.cfg` (keep the order/index aligned with the
  `tokens` enum) and, if it's a new magnitude/class, the matching enum entry in `tokens.h`.
- **New operator:** add its word(s) to `dicionario.cfg` with a delimiter value char (also in
  `STR_DEL` in `config.h`), a `bignum` operation, and a case in the relevant `parse*` function.
- **New error message:** append to `lib/erros.cfg`; reference it by index via `reportError`.

#!/bin/sh
# Recuperação de erro: uma expressão inválida no meio do lote NÃO deve derrubar
# o programa; as expressões válidas seguem sendo resolvidas (linha inválida vira
# "(erro)"). Valida o tratamento de erro recuperável (setjmp/longjmp).
set -u

BIN="${BIN:-build/analisador}"
IN="${IN:-tests/expr_errors.txt}"

if [ ! -x "$BIN" ]; then
    echo "binário não encontrado: $BIN (rode 'make' antes)" >&2
    exit 2
fi

out=$(mktemp)
"$BIN" --batch "$IN" "$out" >/dev/null 2>&1
rc=$?

expected='quinze
(erro)
noventa e nove'
got=$(cat "$out")
rm -f "$out"

if [ "$rc" -eq 0 ] && [ "$got" = "$expected" ]; then
    echo "ERRORS OK (recuperação de erro: lote continua após expressão inválida)"
else
    echo "ERRORS FAIL (rc=$rc)" >&2
    printf 'obtido:\n%s\n' "$got" >&2
    exit 1
fi

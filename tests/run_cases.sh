#!/bin/sh
# Testes de correção: avalia cada expressão de tests/cases.txt via `--eval` e
# compara com o resultado esperado (conferido contra a matemática real).
set -eu

BIN="${BIN:-build/analisador}"
CASES="${CASES:-tests/cases.txt}"

if [ ! -x "$BIN" ]; then
    echo "binário não encontrado: $BIN (rode 'make' antes)" >&2
    exit 2
fi

fail=0
total=0
while IFS='|' read -r expr expected; do
    case "$expr" in ''|\#*) continue ;; esac   # pula vazias e comentários
    total=$((total + 1))
    got=$("$BIN" --eval "$expr" 2>/dev/null)
    if [ "$got" != "$expected" ]; then
        fail=$((fail + 1))
        printf 'FALHOU: "%s"\n  esperado: %s\n  obtido:   %s\n' "$expr" "$expected" "$got" >&2
    fi
done < "$CASES"

if [ "$fail" -eq 0 ]; then
    echo "CASES OK ($total casos)"
else
    echo "CASES FAIL ($fail de $total)" >&2
    exit 1
fi

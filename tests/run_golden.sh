#!/bin/sh
# Harness de regressão golden.
# Roda o modo arquivo de forma não-interativa (lê lib/expressoes.txt, escreve
# resultados.txt) e compara byte-a-byte contra o baseline vigente.
#
# Baseline padrão: tests/resultados.golden.txt (saída limpa, sem os defeitos de
# espaçamento, conferida à mão na Fase 5). O tests/resultados.frozen.txt guarda
# a saída original (com os defeitos) como referência histórica; para comparar
# contra ele use: GOLDEN_BASELINE=tests/resultados.frozen.txt make test
set -eu

BIN="${BIN:-build/analisador}"
BASELINE="${GOLDEN_BASELINE:-tests/resultados.golden.txt}"

if [ ! -x "$BIN" ]; then
    echo "binário não encontrado: $BIN (rode 'make' antes)" >&2
    exit 2
fi
if [ ! -f "$BASELINE" ]; then
    echo "baseline não encontrado: $BASELINE" >&2
    exit 2
fi

# Regenera resultados.txt de forma não-interativa.
rm -f resultados.txt
"$BIN" --batch >/dev/null 2>&1 || true
if [ ! -f resultados.txt ]; then
    echo "resultados.txt não foi gerado pelo --batch" >&2
    exit 1
fi

if diff -u "$BASELINE" resultados.txt; then
    echo "GOLDEN OK (vs $BASELINE)"
else
    echo "GOLDEN FAIL (vs $BASELINE)" >&2
    exit 1
fi

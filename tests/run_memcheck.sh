#!/bin/sh
# Memory check: runs valgrind over the main scenarios and fails on any leak or
# memory error. Use as a routine after changes that touch allocation/ownership.
#
# Requires valgrind (Linux). On macOS/Apple Silicon valgrind is unavailable;
# run it inside a Linux container, e.g.:
#   docker run --rm -v "$PWD":/src -w /src ubuntu:24.04 bash -c \
#     'apt-get update -qq && apt-get install -y -qq build-essential valgrind && make && make memcheck'
set -u

BIN="${BIN:-build/analisador}"
VG="${VALGRIND:-valgrind}"

command -v "$VG" >/dev/null 2>&1 || {
    echo "valgrind nao encontrado (Linux: apt-get install valgrind; macOS: rode em container)" >&2
    exit 2
}
[ -x "$BIN" ] || {
    echo "binario nao encontrado: $BIN (rode 'make' antes)" >&2
    exit 2
}

# --error-exitcode=99 distingue erro de memoria do rc do proprio programa
# (ex.: --eval invalido retorna 1 sem ser vazamento).
VGF="-q --leak-check=full --errors-for-leak-kinds=all --error-exitcode=99"
status=0

check() {
    desc="$1"
    shift
    "$VG" $VGF "$@" >/dev/null 2>/tmp/memcheck.log
    if [ "$?" = "99" ]; then
        echo "MEMCHECK FAIL: $desc" >&2
        cat /tmp/memcheck.log >&2
        status=1
    else
        echo "  ok: $desc"
    fi
}

check "lote valido"        "$BIN" --batch
check "lote com erro"      "$BIN" --batch tests/expr_errors.txt /tmp/memcheck_out.txt
check "eval invalido"      "$BIN" --eval "xyz invalido"
check "eval grande"        "$BIN" --eval "fatorial de quarenta vezes fatorial de quarenta"

if [ "$status" = "0" ]; then
    echo "MEMCHECK OK (0 leaks, 0 erros)"
else
    echo "MEMCHECK FAIL" >&2
fi
exit $status

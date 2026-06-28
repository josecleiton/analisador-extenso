# Makefile — analisador-extenso

CC      ?= cc
CSTD    := -std=c11
WARN    := -Wall -Wextra -Wpedantic
OPT     := -O2
INCLUDE := -Iinclude
CFLAGS  := $(CSTD) $(WARN) $(OPT) $(INCLUDE)
# NOTA: ASan/UBSan ainda travam por causa dos padrões de memória do código
# original (ex.: free(ptr-offset), leituras fora de limite). Reativar
# `-fsanitize=address,undefined` na Fase 6, após a limpeza de buffers/UB.
DBGFLAGS := $(CSTD) $(WARN) -g -O0 $(INCLUDE)
LDLIBS  :=

SRC := $(wildcard src/*.c)

# Release e debug usam diretórios de objetos separados para nunca colidirem
# (objetos instrumentados não vazam para a build release e vice-versa).
OBJ := $(SRC:src/%.c=build/release/%.o)
DBGOBJ := $(SRC:src/%.c=build/debug/%.o)
DEP := $(OBJ:.o=.d) $(DBGOBJ:.o=.d)

BIN := build/analisador
DBGBIN := build/analisador-debug

.PHONY: all debug test clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

build/release/%.o: src/%.c | build/release
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Build de debug (-g -O0). ASan/UBSan voltam na Fase 6 (ver nota em DBGFLAGS).
debug: $(DBGBIN)

$(DBGBIN): $(DBGOBJ)
	$(CC) $(DBGFLAGS) $^ $(LDLIBS) -o $@

build/debug/%.o: src/%.c | build/debug
	$(CC) $(DBGFLAGS) -MMD -MP -c $< -o $@

build/release build/debug:
	mkdir -p $@

# Testes (build release): regressão golden + casos de correção.
test: $(BIN)
	./tests/run_golden.sh
	./tests/run_cases.sh

clean:
	rm -rf build

-include $(DEP)

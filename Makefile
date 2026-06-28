# Makefile — analisador-extenso

CC      ?= cc
CSTD    := -std=c11
WARN    := -Wall -Wextra -Wpedantic
OPT     := -O2
INCLUDE := -Iinclude
CFLAGS  := $(CSTD) $(WARN) $(OPT) $(INCLUDE)
LDLIBS  := -lm

SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/%.o)
DEP := $(OBJ:.o=.d)
BIN := build/analisador

.PHONY: all debug test clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

build:
	mkdir -p build

# Build instrumentado para caçar overflow/UB/vazamento durante o refactor.
debug: CFLAGS := $(CSTD) $(WARN) -g -fsanitize=address,undefined $(INCLUDE)
debug: clean $(BIN)

# Teste de regressão golden (ver tests/run_golden.sh).
test: $(BIN)
	./tests/run_golden.sh

clean:
	rm -rf build

-include $(DEP)

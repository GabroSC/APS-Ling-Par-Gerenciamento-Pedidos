# Makefile — compila lexer+parser da linguagem "pedido"


CC     ?= gcc
BISON  ?= bison
FLEX   ?= flex


CFLAGS  ?= -Wall -Wextra -O2

LDLIBS  ?= -lfl


UNAME_S := $(shell uname 2>/dev/null)
ifeq ($(UNAME_S),Darwin)
  LDLIBS :=
endif

all: pedido


pedido: lex.yy.c pedido.tab.c main.c
	$(CC) $(CFLAGS) -o $@ lex.yy.c pedido.tab.c main.c $(LDLIBS)


pedido.tab.c pedido.tab.h: pedido.y
	$(BISON) -Wall -Wcounterexamples -d $<


lex.yy.c lex.yy.h: pedido.l pedido.tab.h
	$(FLEX) $<


run: pedido
	@ if [ -z "$(FILE)" ]; then echo "Uso: make run FILE=exemplo.ped"; exit 1; fi
	./pedido < "$(FILE)"


tools:
	@echo "CC    = $$(command -v $(CC) || echo 'NÃO ENCONTRADO')"
	@echo "BISON = $$(command -v $(BISON) || echo 'NÃO ENCONTRADO')"
	@echo "FLEX  = $$(command -v $(FLEX) || echo 'NÃO ENCONTRADO')"
	@$(CC) --version | head -n1 || true
	@$(BISON) --version | head -n1 || true
	@$(FLEX) --version  | head -n1 || true

.PHONY: all run clean veryclean tools
clean:
	$(RM) lex.yy.c lex.yy.h pedido.tab.c pedido.tab.h
veryclean: clean
	$(RM) pedido

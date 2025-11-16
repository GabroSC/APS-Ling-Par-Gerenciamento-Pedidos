CC=gcc
CFLAGS=-g -Wall
LEXFLAGS=-Wno-unused-function

OBJS = lex.yy.o parser.tab.o ast.o vm.o valor.o main.o

all: interp

interp: $(OBJS)
	$(CC) -o interp $(OBJS) -lfl

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) $(LEXFLAGS) -c lex.yy.c -o lex.yy.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o lex.yy.c parser.tab.c parser.tab.h interp

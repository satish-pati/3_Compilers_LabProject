CC = gcc
CFLAGS = -w 
LEX = flex
YACC = yacc

all: parser

lexer_test: lexer.l
	$(LEX) lexer.l
	$(CC) $(CFLAGS) lex.yy.c -lfl -o lexer_test

parser: parser.y lexer.l
	$(LEX) lexer.l
	$(YACC) -d parser.y
	$(CC) $(CFLAGS) y.tab.c lex.yy.c -lfl -o parser

clean:
	rm -f lex.yy.c y.tab.c y.tab.h lexer_test parser *.o

test-parser: parser
	./parser $(FILE)
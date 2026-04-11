CC = gcc
CFLAGS = -w 
LEX = flex
YACC = yacc

all: parser parser2
lexer_test: lexer.l
	$(LEX) lexer.l
	$(CC) $(CFLAGS) lex.yy.c -lfl -o lexer_test

parser: parser_unoptimized.y lexer.l
	$(LEX) lexer.l
	$(YACC) -d parser_unoptimized.y
	$(CC) $(CFLAGS) y.tab.c lex.yy.c -lfl -o parser

parser2: parser_optimized.y lexer.l
	$(LEX) lexer.l
	$(YACC) -d parser_optimized.y
	$(CC) $(CFLAGS) y.tab.c lex.yy.c -lfl -o parser2

clean:
	rm -f lex.yy.c y.tab.c y.tab.h lexer_test parser  parser2*.o

test-parser: parser
	./parser $(FILE)

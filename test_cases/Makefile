all: lexer

lexer: lex.yy.c
	gcc lex.yy.c -o lexer -lfl

lex.yy.c: lexer.l
	flex lexer.l

clean:
	rm -f lexer lex.yy.c


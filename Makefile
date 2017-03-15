comp: node.h tokens.h comp.c
	flex func.lex
	gcc -c lex.yy.c
	gcc -c parse.c
	gcc -c print.c
	gcc -c pretty.c
	gcc -c comp.c
	gcc -o comp print.o pretty.o lex.yy.o parse.o comp.o

lexical:
	flex func.lex
	gcc -c lex.yy.c
	gcc -c lex.c
	gcc -c print.c
	gcc -o lexical lex.yy.o print.o lex.o

parser:
	flex func.lex
	gcc -c lex.yy.c
	gcc -c parse.c
	gcc -c print.c
	gcc -c pretty.c
	gcc -o parser print.o pretty.o lex.yy.o parse.o

clean:
	rm -rf *.o simp.c lex.yy.c lexical parser comp

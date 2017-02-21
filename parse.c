#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"
#include "node.h"

int symb;

extern void printSymb();
extern char* showSymb(int);
extern int yylex(void);
extern FILE* yyin;

// prototypes
NODE * program(int depth);
void args(int depth);
NODE * funcs(int depth);
void func(int depth);
void commands(int depth);
void command(int depth);
void assign(int depth);
void if_statement(int depth);
void while_loop(int depth);
void condexpr(int depth);
void bop(int depth);
void exprs(int depth);
void expr(int depth);
void read(int depth);
void write(int depth);

void rule(char * name,int depth)
{	int i;
	for(i=0;i<depth;i++)
		printf(" ");
	printf("%s\n",name);
}

void error(char * rule,char * message)
{	printf("%s: found %s\n",rule,showSymb(symb));
	printf("%s: expected %s\n",rule,message);
	exit(0);
}

void lex()
{	/* printSymb(); */
	symb = yylex();
}

NODE * new_number(int n)
{	NODE * node;
	node = (NODE *) malloc(sizeof(NODE));
	node->tag = NUMBER;
	node->f.value = n;
	return node;
}

NODE * new_name(char * n)
{	NODE * node;
	char * dup = strdup(n);
	node = (NODE *) malloc(sizeof(NODE));
	node->tag = NAME;
	node->f.id = dup;
	return node;
}

NODE * new_node(int tag)
{	NODE * node;
	node = (NODE *) malloc(sizeof(NODE));
	node->tag = tag;
	node->f.b.n1 = NULL;
	node->f.b.n2 = NULL;
	return node;
}


// Le rules...
NODE * program(int depth)
{	rule("program",depth);

	NODE * node;
	if (symb == FUNCTION) {
		node = funcs(depth+1);
	} else {
		error("program","function declaration");
	}

	return node;
}

void args(int depth)
{	rule("args",depth);
	lex(); // eat up NAME

	if (symb == COMMA) {
		lex(); // eat up COMMA
		args(depth);
	}
}

NODE * funcs(int depth)
{	rule("funcs",depth);
	NODE * node;
	node = new_node(SEMI);

	node->f.b.n1 = func(depth+1);

	if (symb == FUNCTION) {
		node->f.b.n2 = funcs(depth);
	}

	return node;
}

void func(int depth)
{	rule("func",depth);
	lex();

	if (symb != NAME) {
		error("func","NAME");
	}

	lex();

	if (symb != LBRA) {
		error("func","(");
	}

	lex();

	if (symb == NAME) {
		args(depth+1);
	} else if (symb != RBRA) {
		error("func",")");
	}

	lex();

	if (symb == RETURNS) {
		lex(); // eat up RETURNS

		if (symb != NAME) {
			error("func","NAME");
		} else {
			lex(); // eat NAME
		}
	}

	if (symb == VARS) {
		lex(); // eat up VARS
		args(depth+1); // eat up args
	}

	if (symb != FBEGIN) {
		error("func","begin");
	}

	lex(); // eat begin
	commands(depth+1);

	if (symb != ENDFUNCTION) {
		error("func","end function");
	}

	lex(); // eat end function

	if (symb != SEMI) {
		error("func",";");
	}

	lex(); // lex SEMI
}

void commands(int depth)
{	rule("commands",depth);
	command(depth+1);

	if (symb == SEMI) {
		lex(); // eat semi
		commands(depth);
	}
}

void command(int depth)
{	rule("command",depth);

	if (symb == NAME) {
		assign(depth+1);
	} else if (symb == IF) {
		if_statement(depth+1);
	} else if (symb == WHILE) {
		while_loop(depth+1);
	} else if (symb == READ) {
		read(depth+1);
	} else if (symb == WRITE) {
		write(depth+1);
	}
}

void assign(int depth)
{	rule("assign",depth);
	lex(); // lex NAME

	if (symb != ASSIGN) {
		error("assign",":=");
	}

	lex(); // lex ASSIGN

	expr(depth+1);
}

void if_statement(int depth)
{	rule("if",depth);
	lex(); // lex IF
	condexpr(depth+1);

	if (symb != THEN) {
		error("if","then");
	}

	lex(); // lex then
	commands(depth+1);

	if (symb == ELSE) {
		lex(); // lex else
		commands(depth+1);
	}

	if (symb != ENDIF) {
		error("if","end if");
	}

	lex(); // lex ENDIF
}

void while_loop(int depth)
{	rule("while",depth);
	lex(); // lex WHILE
	condexpr(depth+1);

	if (symb != LOOP) {
		error("while","loop");
	}

	lex(); // lex LOOP
	commands(depth+1);

	if (symb != ENDLOOP) {
		error("while","end loop");
	}

	lex(); // lex ENDLOOP
}

void condexpr(int depth)
{	rule("condexpr",depth);
	bop(depth+1);

	if (symb != LBRA) {
		error("bop","(");
	}

	lex(); // lex LBRA
	exprs(depth+1);

	if (symb != RBRA) {
		error("bop",")");
	}

	lex(); // lex RBRA
}

void bop(int depth)
{	rule("bop",depth);
	switch (symb){
	case LT:  lex();
		break;
	case LTE: lex();
		break;
	case EQ:  lex();
		break;
	case NEQ: lex();
		break;
	default: error("bop","Less, LessEq, Eq, or Neq");
	}
}

void exprs(int depth)
{	rule("exprs",depth);
	expr(depth);

	if (symb == COMMA) {
		lex(); // eat up COMMA
		expr(depth);
	}
}

void expr(int depth)
{	rule("expr",depth);

	if (symb == NAME) {
		lex(); // lex name

		if (symb == LBRA) {
			lex(); // lex LBRA
			exprs(depth+1);

			if (symb != RBRA) {
				error("expr",")");
			}

			lex(); // lex RBRA
		}
	} else if (symb == NUMBER) {
		lex(); // lex NUMBER
	} else {
		error("assign","NAME or NUMBER");
	}
}

void write(int depth) {
	rule("write",depth);
	lex(); // lex WRITE
	expr(depth+1);
}

void read(int depth) {
	rule("read",depth);
	lex(); // lext READ

	if (symb != NAME) {
		error("read","NAME");
	}

	lex(); // lex NAME
}

int main(int c,char ** argv)
{	if ((yyin=fopen(argv[1],"r"))==NULL) {
		printf("can't open %s\n",argv[1]);
		exit(0);
	}

	symb = yylex();
	program(1);
	fclose(yyin);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"
#include "node.h"

int symb;

extern void printSymb();
extern char* showSymb(int);
extern int yylex(void);
extern char* yytext;
extern FILE* yyin;

/* prototypes */
NODE * program(int depth);
NODE * args(int depth);
NODE * funcs(int depth);
NODE * func(int depth);
NODE * commands(int depth);
NODE * command(int depth);
NODE * assign(int depth);
NODE * if_statement(int depth);
NODE * while_loop(int depth);
NODE * condexpr(int depth);
NODE * bop(int depth);
NODE * exprs(int depth);
NODE * expr(int depth);
NODE * read(int depth);
NODE * write(int depth);

void rule(char * name,int depth)
{/*	int i;
	for(i=0;i<depth;i++)
		printf(" ");
	printf("%s\n",name); */
}

void error(char * rule,char * message)
{	fprintf(stderr, "%s: found %s\n",rule,showSymb(symb));
	fprintf(stderr, "%s: expected %s\n",rule,message);
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


/* Le rules... */
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

NODE * args(int depth)
{	rule("args",depth);
	NODE * node;
	node = new_node(COMMA);
	node->f.b.n1 = new_name(yytext);
	lex(); /* eat up NAME */

	if (symb == COMMA) {
		lex(); /* eat up COMMA */
		node->f.b.n2 = args(depth);
	}

	return node;
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

NODE * func(int depth)
{	rule("func",depth);

	lex();
	NODE * node, * definition, * arguments, * returns, * variables;
	arguments = NULL;
	returns = NULL;
	variables = NULL;
	node = new_node(FUNCTION);
	definition = new_node(FBEGIN); /* holds non option info (name + cmds) */
	node->f.b.n2 = definition;
	if (symb != NAME) {
		error("func","NAME");
	}

	definition->f.b.n1 = new_name(yytext);

	lex();

	if (symb != LBRA) {
		error("func","(");
	}

	lex();

	if (symb == NAME) {
		arguments = args(depth+1);
	}

	if (symb != RBRA) {
		error("func",")");
	}

	lex();

	if (symb == RETURNS) {
		lex(); /* eat up RETURNS */

		if (symb != NAME) {
			error("func","NAME");
		} else {
			returns = new_node(RETURNS);
			returns->f.b.n1 = new_name(yytext);
			lex(); /* eat NAME */
		}
	}

	if (symb == VARS) {
		lex(); /* eat up VARS */
		variables = new_node(VARS);
		variables->f.b.n1 = args(depth+1); /* eat up args */
	}

	if (symb != FBEGIN) {
		error("func","begin");
	}

	lex(); /* eat begin */

	definition->f.b.n2 = commands(depth+1);

	if (symb != ENDFUNCTION) {
		error("func","end function");
	}

	lex(); /* eat end function */

	if (symb != SEMI) {
		error("func",";");
	}

	lex(); /* lex SEMI */

	NODE * optional_return = new_node(RBRA);
	optional_return->f.b.n1 = returns;
	optional_return->f.b.n2 = variables;

	NODE * optional_args = new_node(RBRA);
	optional_args->f.b.n1 = arguments;
	optional_args->f.b.n2 = optional_return;

	node->f.b.n1 = optional_args;

	return node;
}

NODE * commands(int depth)
{	rule("commands",depth);
	NODE * node;
	node = new_node(SEMI);

	node->f.b.n1 = command(depth+1);

	if (symb == SEMI) {
		lex(); /* eat semi */
		/* check if we have more commands... */
		if(symb == NAME || symb == IF || symb == WHILE || symb == READ || symb == WRITE)
			node->f.b.n2 = commands(depth);
	} else {
		error("commands", ";");
	}

	return node;
}

NODE * command(int depth)
{	rule("command",depth);

	switch(symb) {
	case NAME:
		return assign(depth+1);
	case IF:
		return if_statement(depth+1);
	case WHILE:
		return while_loop(depth+1);
	case READ:
		return read(depth+1);
	case WRITE:
		return write(depth+1);
	default:
		return NULL;
	}
}

NODE * assign(int depth)
{	rule("assign",depth);
	NODE * node;
	node = new_node(ASSIGN);

	node->f.b.n1 = new_name(yytext);

	lex(); /* lex NAME */


	if (symb != ASSIGN) {
		error("assign",":=");
	}

	lex(); /* lex ASSIGN */

	node->f.b.n2 = expr(depth+1);

	return node;
}

NODE * if_statement(int depth)
{	rule("if",depth);
	lex(); /* lex IF */

	NODE * node, * if_commands, * else_node;
	node = new_node(IF);

	node->f.b.n1 = condexpr(depth+1);

	if (symb != THEN) {
		error("if","then");
	}

	lex(); /* lex then */
	if_commands = commands(depth+1);
	node->f.b.n2 = if_commands;

	if (symb == ELSE) {
		lex(); /* lex else */
		else_node = new_node(ELSE);
		node->f.b.n2 = else_node;
		else_node->f.b.n1 = if_commands;
		else_node->f.b.n2 = commands(depth+1);
	}

	if (symb != ENDIF) {
		error("if","end if");
	}

	lex(); /* lex ENDIF */

	return node;
}

NODE * while_loop(int depth)
{	rule("while",depth);
	lex(); /* lex WHILE */
	NODE * node;
	node = new_node(WHILE);

	node->f.b.n1 = condexpr(depth+1);

	if (symb != LOOP) {
		error("while","loop");
	}

	lex(); /* lex LOOP */
	node->f.b.n2 = commands(depth+1);

	if (symb != ENDLOOP) {
		error("while","end loop");
	}

	lex(); /* lex ENDLOOP */

	return node;
}

NODE * condexpr(int depth)
{	rule("condexpr",depth);
	NODE * node;
	node = bop(depth+1);

	if (symb != LBRA) {
		error("bop","(");
	}

	lex(); /* lex LBRA */
	node->f.b.n1 = exprs(depth+1);

	if (symb != RBRA) {
		error("bop",")");
	}

	lex(); /* lex RBRA */

	return node;
}

NODE * bop(int depth)
{	rule("bop",depth);
	NODE * node;

	switch (symb){
	case LT:
	case LTE:
	case EQ:
	case NEQ:
		node = new_node(symb);
		break;
	default: error("bop","Less, LessEq, Eq, or Neq");
	}

	lex();

	return node;
}

NODE * exprs(int depth)
{	rule("exprs",depth);
	NODE * node;
	node = new_node(COMMA);
	node->f.b.n1 = expr(depth);

	if (symb == COMMA) {
		lex(); /* eat up COMMA */
		node->f.b.n2 = exprs(depth);
	}

	return node;
}

NODE * expr(int depth)
{	rule("expr",depth);
	NODE * node;

	if (symb == NAME) {
		node = new_name(yytext);
		lex(); /* lex name */

		if (symb == LBRA) {
			NODE * function = new_node(FUNCTION);
			function->f.b.n1 = node;
			node = function;
			lex(); /* lex LBRA */
			node->f.b.n2 = exprs(depth+1);

			if (symb != RBRA) {
				error("expr",")");
			}

			lex(); /* lex RBRA */
		}
	} else if (symb == NUMBER) {
		node = new_number(atoi(yytext));
		lex(); /* lex NUMBER */
	} else {
		error("assign","NAME or NUMBER");
	}

	return node;
}

NODE * write(int depth)
{
	rule("write",depth);
	NODE * node;
	node = new_node(WRITE);
	lex(); /* lex WRITE */
	node->f.b.n1 = expr(depth+1);

	return node;
}

NODE * read(int depth)
{
	rule("read",depth);
	lex(); /* lext READ */
	NODE * node;
	node = new_node(READ);

	if (symb != NAME) {
		error("read","NAME");
	}

	node->f.b.n1 = new_name(yytext);
	lex(); /* lex NAME */

	return node;
}

/*
int main(int c,char ** argv)
{	if ((yyin=fopen(argv[1],"r"))==NULL) {
		printf("can't open %s\n",argv[1]);
		exit(0);
	}

	symb = yylex();
	NODE * tree = program(1);
	fclose(yyin);
	prettytree(tree, 0);
	return 0;
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"
#include "node.h"


extern int symb;
extern int yylex(void);
extern NODE * program(int depth);
extern FILE * yyin;
extern char * showSymb(int);

/* our registers */
#define MAXREG 20		/* $s0-$s7, $t0-$t7, $a0-$a3 */
#define E1 MAXREG		/* $t8 */
#define E2 MAXREG+1		/* $t9 */
#define V0 MAXREG+2		/* $v0 */
char * registers[MAXREG];	/* store what var is assocd to a register */
int rp, label_no;		/* track next free register index, and what
				 * label number we are on. */

/* return index of register where a var is stored
 * returns -1 if no var found */
int findvar(char * id)
{
	int i;
	for(i=0; i<rp; i++) {
		if(strcmp(id, registers[i]) == 0) {
			return i;
		}
	}
	return -1;
}

/* return name of a register associated with an index */
char * regname(int r)
{
	char rnum[3];
	switch(r) {
	case E1: return "$t8";
	case E2: return "$t9";
	case R0: return "$r0";
	default:
		if(r<8) {
			sprintf(rnum, "$s%d", r);
		} else if(r<16) {
			sprintf(rnum, "$t%d", r);
		} else {
			sprintf(rnum, "$a%d", r);
		}
		return strdup(rnum);
	}
}

NODE * find_function(char * func_name, NODE * program) {
	NODE * current = program;
	NODE * func, * name;
	char * f_name;
	while(1) {
		func = current->f.b.n1;
		name = (func->f.b.n2)->f.b.n1;
		f_name = name->f.id;
		if(strcmp(func_name, f_name) == 0) return func;
		if(current->f.b.n2 == NULL) break;
		current = current->f.b.n2;
	}

	/* if we get here then function does not exist!! */
	fprintf(stderr, "comp.c: find_function error - function '%s' not defined\n", func_name);
	exit(1);
}

/* helper to remove main func from the AST (makes it easier to iterate later on */
NODE * remove_function(NODE * program, NODE * main)
{
	NODE * previous = NULL;
	NODE * current = program;
	while(1) {
		if(current->f.b.n1 == main) {
			if(previous == NULL) {
				return current->f.b.n2;
			} else {
				/* get rid of main */
				previous->f.b.n2 = current->f.b.n2;
				return program;
			}
			break;
		} else {
			previous = current;
			current = current->f.b.n2;
		}
	}

	return program;
}

void compile_func(NODE * func)
{}

void compile_funcs(NODE * funcs)
{}

void compile_program(NODE * program)
{
	NODE * main = find_function("Main", program);
	NODE * funcs = remove_function(program, main);
	/* print assembly things */
	printf("\t.data\n");
	printf("sinp:\t.asciiz \"INPUT> \"\n");
	printf("\n\t.text\n");
	printf("main:");
	/* print out main */
	compile_func(main);
	/* terminate program */
	printf("\tli $v0, 10\n");
	printf("\tsyscall\n\n");

	/* print out all other functions... */
	compile_funcs(funcs);

	return;
}

int main(int argc, char ** argv)
{
	if(argc == 1) {
		printf("comp.c: Usage ./comp <filename>\n");
		exit(1);
	}

	if((yyin=fopen(argv[1],"r"))==NULL) {
		perror("comp.c");
		exit(1);
	}

	symb = yylex();
	NODE * tree = program(1);
	fprintf(stderr, "Parsed: %s\ncompiling...\n", argv[1]);
	fclose(yyin);
	rp = 0;
	label_no = 0;
	compile_program(tree);
	fprintf(stderr, "compiled.\n");
	return 0;
}


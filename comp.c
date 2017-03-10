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
#define MAXREG 16		/* $s0-$s7, $t0-$t7 */
#define A0 MAXREG		/* $a0 */
#define A1 MAXREG+1		/* $a1 */
#define A2 MAXREG+2		/* $a2 */
#define A3 MAXREG+3		/* $a3 */
#define E1 MAXREG+4		/* $t8 */
#define E2 MAXREG+5		/* $t9 */
#define V0 MAXREG+6		/* $v0 */
char * registers[MAXREG];	/* store what var is assocd to a register */
char * argv[4];
char * v0s;
int rp, rb, label_no, argc;		/* track next free register index, and what
								 * label number we are on. */

/* return index of register where a var is stored
 * returns -1 if no var found */
int findvar(char * id)
{
	int i;
	for(i=rb; i<rp; i++) {
		if(strcmp(id, registers[i]) == 0) {
			return i;
		}
	}

	for(i=MAXREG; i<(argc+MAXREG); i++) {
		argv[i-MAXREG];
		if(strcmp(id, argv[i-MAXREG]) == 0) {
			return i;
		}
	}

	if(strcmp(id, v0s) == 0) {
		return V0;
	}

	return -1;
}

/* return name of a register associated with an index */
char * regname(int r)
{
	char rnum[4];
	switch(r) {
	case E1: return "$t8";
	case E2: return "$t9";
	case A0: return "$a0";
	case A1: return "$a1";
	case A2: return "$a2";
	case A3: return "$a3";
	case V0: return "$v0";
	default:
		if(r<8) {
			sprintf(rnum, "$s%d", r);
		} else {
			sprintf(rnum, "$t%d", r-8);
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

void compile_expr(NODE * expr)
{
	int var;
	switch(expr->tag) {
	case NAME:
		var = findvar(expr->f.id);
		if(var == -1) {
			fprintf(stderr, "Undeclared variable %s\n", expr->f.id);
		}
		printf("\tadd %s %s 0\n", regname(E2), regname(var));
		break;
	case NUMBER:
		printf("\tli %s %d\n", regname(E2), expr->f.value);
		break;
	case FUNCTION:
		printf("\tfunction call\n");
		break;
	default:
		fprintf(stderr, "Malformed expression: %s\n", showSymb(expr->tag));
		exit(1);
	}
}

void compile_assign(NODE * assign)
{
	char * var = (assign->f.b.n1)->f.id;
	int reg = findvar(var);
	if(reg == -1) {
		fprintf(stderr, "variable does not exist: %s\n", var);
	}
	char * reg_s = regname(reg);

	/* evaluate expr into E2 */
	compile_expr(assign->f.b.n2);

	/* move E2 to reg */
	printf("\tadd %s %s 0\n", regname(reg), regname(E2));
}

void compile_if(NODE * if_command)
{}

void compile_while(NODE * while_command)
{}

void compile_write(NODE * write)
{}

void compile_read(NODE * read)
{}

void compile_command(NODE * command)
{
	switch(command->tag){
	case ASSIGN:
		compile_assign(command);
		break;
	case IF:
		compile_if(command);
		break;
	case WHILE:
		compile_while(command);
		break;
	case WRITE:
		compile_write(command);
		break;
	case READ:
		compile_read(command);
		break;
	default:
		fprintf(stderr, "unrecognised command: %s\n", showSymb(command->tag));
		exit(1);
	}
}

void compile_commands(NODE * commands)
{
	compile_command(commands->f.b.n1);

	if(commands->f.b.n2 != NULL) {
		compile_commands(commands->f.b.n2);
	}
}

void compile_func(NODE * func)
{
	NODE *args, *vars, *commands, *returns;
	args = (func->f.b.n1)->f.b.n1;
	returns = ((func->f.b.n1)->f.b.n2)->f.b.n1;
	vars = ((func->f.b.n1)->f.b.n2)->f.b.n2;
	commands = (func->f.b.n2)->f.b.n2;
	/* ASSIGN ARGS */
	argc = 0;
	if(args != NULL) {
		NODE * current =  args;
		while(1) {
			if(argc > 3) {
				fprintf(stderr, "Max 4 args allowed.\n");
				exit(1);
			}

			argv[argc] = strdup((current->f.b.n1)->f.id);
			argc++;
			fprintf(stderr, "Assigning %s to register %s\n",
					argv[argc-1],
					regname(findvar(argv[argc-1])));

			if(current->f.b.n2 == NULL) {
				break;
			} else {
				current = current->f.b.n2;
			}
		}
	}
	/* ASSIGN RETURNS */
	v0s = "";
	if(returns != NULL) {
		v0s = strdup(returns->f.b.n1->f.id);
		fprintf(stderr, "Assigning %s to register %s\n",
				v0s,
				regname(findvar(v0s)));
	}
	/* ASSIGN VARS */
	if(vars != NULL) {
		NODE * current = vars->f.b.n1;
		int i = 0;
		while(1) {
			if(i>7) {
				fprintf(stderr, "Max 8 vars allowed\n");
				exit(1);
			}
			registers[rp] = strdup((current->f.b.n1)->f.id);
			rp++;
			i++;
			fprintf(stderr, "Assigning %s to register %s\n",
					registers[rp-1],
					regname(findvar(registers[rp-1])));

			if(current->f.b.n2 == NULL) {
				break;
			} else {
				current = current->f.b.n2;
			}
		}
	}
	/* COMMANDS */
	compile_commands(commands);
}

void compile_funcs(NODE * funcs)
{
	char * name = (((funcs->f.b.n1)->f.b.n2)->f.b.n1)->f.id;
	printf("%s:", name);
	rp = 8;
	rb = 8;
	compile_func(funcs->f.b.n1);
	if(funcs->f.b.n2 != NULL) {
		printf("\n");
		compile_funcs(funcs->f.b.n2);
	}
}


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
	rp = 0;
	rb = 0;
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


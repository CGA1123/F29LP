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
}


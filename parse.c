#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

int symb;

extern printSymb();
extern char* showSymb(int);
extern int yylex(void);
extern FILE* yyin;

rule(char * name,int depth)
{ int i;
  for(i=0;i<depth;i++)
    printf(" ");
  printf("%s\n",name);
}

error(char * rule,char * message)
{ printf("%s: found %s\n",rule,showSymb(symb));
  printf("%s: %s\n",rule,message);
  exit(0);
}

lex()
{ printSymb();
  symb = yylex();
}

// Le rules...

main(int c,char ** argv)
{
  if((yyin=fopen(argv[1],"r"))==NULL){
    printf("can't open %s\n",argv[1]);
    exit(0);
  }
  symb = yylex();
  program(1);
  fclose(yyin);
}

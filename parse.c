#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

int symb;

extern printSymb();
extern char* showSymb(int);
extern int yylex(void);
extern FILE* yyin;

// prototypes
void program(int depth);
void args(int depth);
void funcs(int depth);
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


void rule(char * name,int depth)
{ int i;
  for(i=0;i<depth;i++)
    printf(" ");
  printf("%s\n",name);
}

void error(char * rule,char * message)
{ printf("%s: found %s\n",rule,showSymb(symb));
  printf("%s: expected %s\n",rule,message);
  exit(0);
}

void lex()
{ printSymb();
  symb = yylex();
}

// Le rules...
void program(int depth)
{ rule("program",depth);
  if(symb == FUNCTION) {
    funcs(depth+1);
  } else {
    error("program","function declaration");
  }
}

void args(int depth)
{ rule("args",depth);
  lex(); // eat up NAME
  if(symb == COMMA) {
    lex(); // eat up COMMA
    args(depth);
  }
}

void funcs(int depth)
{ rule("funcs",depth);
  func(depth+1);
  lex();
  if(symb != SEMI) {
    error("funcs",";");
  }
  lex();
  if(symb == FUNCTION) {
    func(depth+1);
  }
}

void func(int depth)
{ rule("func",depth);
  lex();
  if(symb != NAME) {
    error("func","NAME");
  }
  lex();
  if(symb != LBRA) {
    error("func","(");
  }
  lex();
  if(symb == NAME) {
    args(depth+1);
  } else if(symb != RBRA) {
    error("func",")");
  }
  lex();
  if(symb == RETURNS) {
    lex(); // eat up RETURNS
    if(symb != NAME) {
      error("func","NAME");
    } else {
      lex(); // eat NAME
    }
  } else if(symb == VARS) {
    lex(); // eat up VARS
    args(depth+1); // eat up args
  } else if(symb != FBEGIN) {
    error("func","begin");
  }
  lex(); // eat begin
  commands(depth+1);
  if(symb != ENDFUNCTION) {
    error("func","end function");
  }
  lex(); // eat end function
}

void commands(int depth)
{ rule("commands",depth);
  command(depth+1);
  if(symb == SEMI) {
    lex(); // eat semi
    command(depth+1);
  }
}

void command(int depth)
{ rule("command",depth);
  if(symb == NAME) {
    assign(depth+1);
  } else if(symb == IF) {
    if_statement(depth+1);
  } else if(symb == WHILE) {
    while_loop(depth+1);
  } else if(symb == READ) {
    read(depth+1);
  } else if(symb == WRITE) {
    write(depth+1);
  } else {
    error("command","assign, if, while, read, or write");
  }
}

void assign(int depth)
{ rule("assign",depth);
}

void if_statement(int depth)
{ rule("if",depth);
}

void while_loop(int depth)
{ rule("while",depth);
}

void condexpr(int depth)
{ rule("condexpr",depth);
}

void bop(int depth)
{ rule("bop",depth);
}

void exprs(int depth)
{ rule("exprs",depth);
}

void expr(int depth)
{ rule("expr",depth);
}

int main(int c,char ** argv)
{
  if((yyin=fopen(argv[1],"r"))==NULL){
    printf("can't open %s\n",argv[1]);
    exit(0);
  }
  symb = yylex();
  program(1);
  fclose(yyin);
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

extern int symb;
extern char * yytext;

char * showSymb(int symb)
{
  switch(symb)
  {
    case  FUNCTION: return "function";
    case  RETURNS: return "returns";
    case  VARS: return "variables";
    case  FBEGIN: return "begin";
    case  ENDFUNCTION: return "end function";
    case  READ: return "read";
    case  WRITE: return "write";
    case  IF: return "if";
    case  THEN: return "then";
    case  ELSE: return "else";
    case  ENDIF: return "end if";
    case  WHILE: return "while";
    case  LOOP: return "loop";
    case  ENDLOOP: return "end loop";
    case  LT: return "Less";
    case  LTE: return "LessEq";
    case  EQ: return "Eq";
    case  NEQ: return "NEq";
    case  NUMBER: return "NUMBER";
    case  NAME: return "NAME";
    case  SEMI: return ";";
    case  COMMA: return ",";
    case  LBRA: return "(";
    case  RBRA: return ")";
    case  ASSIGN: return ":=";
    case  EOF: return "EOF";
    default: printf("bad symbol: %d",symb); return 0;
  }
}

void printSymb()
{
  char * s;
  printf("%s ",showSymb(symb));
  if(symb == NAME || symb == NUMBER) {
    if(yytext == NULL) {
      printf("Error: yytext is null");
    } else {
      printf("%s\n",yytext);
    }
  } else {
    printf("\n");
  }
}

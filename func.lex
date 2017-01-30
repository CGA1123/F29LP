%{
#include "tokens.h"
%}

NUMBER [0-9]
NAME	 [a-zA-Z][A-Za-z0-9]*

%%

";" 	          { return SEMI; }
":=" 	          { return ASSIGN; }
","             { return COMMA; }
"("             { return LBRA; }
")"             { return RBRA; }
"if"	          { return IF; }
"then"	        { return THEN; }
"else"	        { return ELSE; }
"end if"        { return ENDIF; }
"begin"	        { return FBEGIN; }
"function"      { return FUNCTION; }
"end function"  { return ENDFUNCTION; }
"variables"     { return VARS; }
"returns"       { return RETURNS; }
"write"         { return WRITE; }
"read"          { return READ; }
"while"         { return WHILE; }
"loop"          { return LOOP; }
"end loop"      { return ENDLOOP; }
"Less"          { return LT; }
"LessEq"        { return LTE; }
"Eq"            { return EQ; }
"NEq"           { return NEQ; }

{NUMBER}+       { return NUMBER; }
{NAME}	        { return NAME; }
<<EOF>>	        { return EOF; }

[ \t\n]+          /* eat up whitespace */


%%

int yywrap() { return EOF; }

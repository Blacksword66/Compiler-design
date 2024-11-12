%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

int a_count = 0;
int b_count = 0;
%}

%%
a       { a_count++; return A; }
b       { b_count++; return B; }
\n      { return NL; }
.       { return yytext[0]; }
%%

/* Remove yywrap from lex file since it's defined in yacc */
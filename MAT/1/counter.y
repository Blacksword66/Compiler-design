%{
#include <stdio.h>
#include <stdlib.h>

extern int a_count;
extern int b_count;

/* Add all necessary function declarations */
int yylex(void);
int yyparse(void);
void yyerror(const char *str);

void yyerror(const char *str) {
    fprintf(stderr, "error: %s\n", str);
}

int yywrap(void) {
    return 1;
}

int main() {
    yyparse();
    return 0;
}
%}

%token A B NL

%%
input:  /* empty */
    | input line
    ;

line:   string NL {
        if (a_count != b_count) {
            printf("String accepted!\n");
        } else {
            printf("String rejected!\n");
        }
        a_count = 0;
        b_count = 0;
    }
    ;

string: /* empty */
    | string A
    | string B
    ;
%%
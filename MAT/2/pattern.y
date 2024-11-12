%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex(void);

%}

%token A B NL

%%
input:  /* empty */
    | input line
    ;

line: pattern NL {
        printf("String accepted!\n");
    }
    | error NL {
        printf("String rejected!\n");
        yyerrok;
    }
    ;

pattern: ab bbaa_part bba ba_part
    ;

ab: A B
    ;

bbaa_part: /* empty */
    | bbaa_part B B A A
    ;

bba: B B A
    ;

ba_part: /* empty */
    | ba_part B A
    ;

%%

void yyerror(const char *s) {
    //fprintf(stderr, "%s\n", s);
}

int main(void) {
    printf("Enter strings to test (Ctrl+D to exit):\n");
    printf("Pattern should match: ab(bbaa)^n bba(ba)^n where n >= 0\n");
    yyparse();
    return 0;
}
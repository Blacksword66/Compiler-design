%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s);
int yylex(void);
extern int lineno;
%}
%token NUMBER NEWLINE
%token PLUS MINUS TIMES DIVIDE
%token LPAREN RPAREN LCURLY RCURLY LSQUARE RSQUARE MOD
%%
input:
    /* Empty */
    | input line
    ;

line: NEWLINE
    | expr NEWLINE { printf("Result: %d\n", $1); }
    ;

expr: term
    | expr PLUS term { $$ = $1 + $3; }
    | expr MINUS term { $$ = $1 - $3; }
    ;

term: factor
    | term TIMES factor { $$ = $1 * $3; }
    | term DIVIDE factor {
        if ($3 == 0) {
            yyerror("Division by zero!");
            YYABORT;
        }
        $$ = $1 / $3;
    }
    | term MOD factor {
        if ($3 == 0) {
            yyerror("Modulo by zero!");
            YYABORT;
        }
        $$ = $1 % $3;
    }
    ;

factor: NUMBER { $$ = $1; }
    | MINUS factor { $$ = -$2; }
    | LPAREN expr RPAREN { $$ = $2; }
    | LCURLY expr RCURLY { $$ = $2; }
    | LSQUARE expr RSQUARE { $$ = $2; }
    ;
%%
void yyerror(const char *s) {
    fprintf(stderr, "Error at line %d: %s\n", lineno, s);
}
int main(void) {
    printf("Enter expressions (press Ctrl+D to exit):\n");
    yyparse();
    return 0;
}
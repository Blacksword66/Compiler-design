%{
  #include <stdio.h>
%}

%%

"if"|"else"|"while"|"return" { 
  printf("KEYWORD: %s\n", yytext);
}

[a-zA-Z_][a-zA-Z0-9_]* { 
  printf("IDENTIFIER: %s\n", yytext);
}

[0-9]+ { 
  printf("NUMBER: %s\n", yytext);
}

[=+-/*<>] {
  printf("OPERATOR: %s\n", yytext);
}

";" {
  printf("SEMICOLON: %s\n", yytext);
}

"("|")" {
  printf("PARENTHESIS: %s\n", yytext);
}

"{"|"}" {
  printf("CURLY BRACE: %s\n", yytext);
}

. {
  /* Ignore other characters */
}

%%

int main() {
  FILE *fp = fopen("program.txt", "r");
  if (!fp) {
      printf("Could not open file\n");
      return 1;
  }
  yyin = fp;
  yylex();
  fclose(fp);
  return 0;
}

int yywrap() {
  return -1;
}

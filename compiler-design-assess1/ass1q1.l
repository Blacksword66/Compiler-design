%{
#include <stdio.h>
int state = 0; // initial state is q0
%}

%%

0 {
    if (state == 0) state = 1;
    else if (state == 1) state = 0;
    else if (state == 2) state = 3;
    else if (state == 3) state = 2;
}

1 {
    if (state == 0) state = 2;
    else if (state == 1) state = 3;
    else if (state == 2) state = 0;
    else if (state == 3) state = 1;
}

\n {
    if (state == 1) printf("Accepted\n");
    else printf("Rejected\n");
    state = 0; // reset state for the next input
}

. { /* Ignore any other characters */ }

%%

int main() {
    yylex();
    return 0;
}

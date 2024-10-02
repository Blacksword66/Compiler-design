#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10

// Parse table with production numbers assigned
int parseTable[MAX][MAX] = {
    {1, 0, 0, 1, 0, 0}, // E -> TH       (Production 1)
    {0, 2, 0, 0, 3, 3}, // H -> +TH | @  (Productions 2 and 3)
    {4, 0, 0, 4, 0, 0}, // T -> FI      (Production 4)
    {0, 6, 5, 0, 6, 6}, // I -> *FI | @  (Productions 5 and 6)
    {8, 0, 0, 7, 0, 0}  // F -> (E) | i  (Productions 7 and 8)
};

// Grammar rules
char *production[] = {
    "E -> TH",
    "H -> +TH",
    "H -> @", // Epsilon production replaced with @
    "T -> FI",
    "I -> *FI",
    "I -> @", // Epsilon production replaced with @
    "F -> (E)",
    "F -> i"
};

// Stack for parsing
char stack[MAX];
int top = -1;

// Function to push to stack
void push(char symbol) {
    if (top == MAX - 1) {
        printf("Stack Overflow\n");
    } else {
        stack[++top] = symbol;
    }
}

// Function to pop from stack
char pop() {
    if (top == -1) {
        printf("Stack Underflow\n");
        return '\0';
    } else {
        return stack[top--];
    }
}

// Function to check if stack is empty
int isEmpty() {
    return top == -1;
}

// Function to print stack contents and the remaining input
void printStack(char input[], int inputIndex) {
    printf("Stack: ");
    for (int i = top; i >= 0; i--) {
        printf("%c", stack[i]);
    }
    printf("\nInput: ");
    for (int i = inputIndex; input[i] != '\0'; i++) {
        printf("%c", input[i]);
    }
    printf("\n");
}

// Function to get terminal index
int getTerminalIndex(char symbol) {
    switch (symbol) {
        case '+': return 1; // Terminal index for '+'
        case '*': return 2; // Terminal index for '*'
        case '(': return 3; // Terminal index for '('
        case ')': return 4; // Terminal index for ')'
        case 'i': return 0; // 'i' for identifier (id)
        case '$': return 5; // End of input
        default: return -1; // Invalid terminal
    }
}

// Function to get non-terminal index
int getNonTerminalIndex(char symbol) {
    switch (symbol) {
        case 'E': return 0; // Non-terminal index for E
        case 'H': return 1; // Replacing E' with H
        case 'T': return 2; // Non-terminal index for T
        case 'I': return 3; // Replacing T' with I
        case 'F': return 4; // Non-terminal index for F
        default: return -1; // Invalid non-terminal
    }
}

// Predictive parsing function
void predictiveParsing(char input[]) {
    int i = 0; // Input index
    push('$'); // Push end of input symbol
    push('E'); // Push start symbol

    while (!isEmpty()) {
        printStack(input, i); // Show the current stack content and input state
        char topStack = stack[top]; // Get top of stack
        char currentInput = input[i]; // Get current input character

        // Check if both current input and top of stack are '$'
        if (currentInput == '$' && topStack == '$') {
            printf("Input string is successfully parsed.\n");
            return;
        }

        if (topStack == currentInput) {
            // Terminal matches, consume input
            printf("Match: %c\n", currentInput);
            pop();
            i++;
        } else if (isupper(topStack)) {
            // Non-terminal, use parse table
            int row = getNonTerminalIndex(topStack);
            int col = getTerminalIndex(currentInput);

            if (row != -1 && col != -1 && parseTable[row][col] != 0) {
                pop(); // Pop the non-terminal

                // Get the production from the table
                printf("Production used: %s\n", production[parseTable[row][col] - 1]);

                // Push the production to the stack in reverse order
                char *prod = production[parseTable[row][col] - 1];
                for (int j = strlen(prod) - 1; j > 3; j--) { // Start from index 3 to skip "X -> "
                    if (prod[j] != ' ' && prod[j] != '@') { // Check for epsilon
                        push(prod[j]);
                    }
                }
            } else {
                printf("Error: No matching production found for %c\n", topStack);
                return;
            }
        } else {
            printf("Error: Unexpected terminal symbol at stack: %c and input: %c\n", topStack, currentInput);
            return;
        }
    }

    // Final check to ensure successful parsing if not already checked
    if (input[i] == '$') {
        printf("Input string is successfully parsed.\n");
    } else {
        printf("Error: Input string is not fully consumed.\n");
    }
}

int main() {
    char input[50];

    // Input the string to parse
    printf("Enter the input string (terminated by $): ");
    scanf("%s", input);

    // Call predictive parsing function
    predictiveParsing(input);

    return 0;
}

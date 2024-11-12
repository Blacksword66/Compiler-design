#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 10
#define GRAMMAR_SIZE 20
#define TERMINAL_SIZE 10

// Stack for parsing
char stack[MAX];
int top = -1;

char terminals[TERMINAL_SIZE];
char nonTerminals[TERMINAL_SIZE];
int parseTable[MAX][MAX]; // Dynamic parse table

// Grammar rules
char *production[GRAMMAR_SIZE];
int productionCount = 0;

// Function to push to stack
void push(char symbol)
{
    if (top == MAX - 1)
    {
        printf("Stack Overflow\n");
    }
    else
    {
        stack[++top] = symbol;
    }
}

// Function to pop from stack
char pop()
{
    if (top == -1)
    {
        printf("Stack Underflow\n");
        return '\0';
    }
    else
    {
        return stack[top--];
    }
}

// Function to check if stack is empty
int isEmpty()
{
    return top == -1;
}

// Function to print stack contents and the remaining input
void printStack(char input[], int inputIndex)
{
    printf("Stack: ");
    for (int i = top; i >= 0; i--)
    {
        printf("%c", stack[i]);
    }
    printf("\nInput: ");
    for (int i = inputIndex; input[i] != '\0'; i++)
    {
        printf("%c", input[i]);
    }
    printf("\n");
}

// Function to get terminal index
int getTerminalIndex(char symbol)
{
    for (int i = 0; i < TERMINAL_SIZE; i++)
    {
        if (terminals[i] == symbol)
        {
            return i;
        }
    }
    return -1; // Invalid terminal
}

// Function to get non-terminal index
int getNonTerminalIndex(char symbol)
{
    for (int i = 0; i < TERMINAL_SIZE; i++)
    {
        if (nonTerminals[i] == symbol)
        {
            return i;
        }
    }
    return -1; // Invalid non-terminal
}

// Predictive parsing function
void predictiveParsing(char input[])
{
    int i = 0;             // Input index
    push('$');             // Push end of input symbol
    push(nonTerminals[0]); // Push start symbol (first non-terminal)

    while (!isEmpty())
    {
        printStack(input, i);         // Show the current stack content and input state
        char topStack = stack[top];   // Get top of stack
        char currentInput = input[i]; // Get current input character

        // Check if both current input and top of stack are '$'
        if (currentInput == '$' && topStack == '$')
        {
            printf("Input string is successfully parsed.\n");
            return;
        }

        if (topStack == currentInput)
        {
            // Terminal matches, consume input
            printf("Match: %c\n", currentInput);
            pop();
            i++;
        }
        else if (isupper(topStack))
        {
            // Non-terminal, use parse table
            int row = getNonTerminalIndex(topStack);
            int col = getTerminalIndex(currentInput);

            if (row != -1 && col != -1 && parseTable[row][col] != 0)
            {
                pop(); // Pop the non-terminal

                // Get the production from the table
                printf("Production used: %s\n", production[parseTable[row][col] - 1]);

                // Push the production to the stack in reverse order
                char *prod = production[parseTable[row][col] - 1];
                for (int j = strlen(prod) - 1; j > 3; j--)
                { // Start from index 3 to skip "X -> "
                    if (prod[j] != ' ' && prod[j] != 'e')
                    { // Check for epsilon
                        push(prod[j]);
                    }
                }
            }
            else
            {
                printf("Error: No matching production found for %c\n", topStack);
                return;
            }
        }
        else
        {
            printf("Error: Unexpected terminal symbol at stack: %c and input: %c\n", topStack, currentInput);
            return;
        }
    }

    // Final check to ensure successful parsing if not already checked
    if (input[i] == '$')
    {
        printf("Input string is successfully parsed.\n");
    }
    else
    {
        printf("Error: Input string is not fully consumed.\n");
    }
}

// Function to get the grammar and build parse table
// Function to get the grammar and build parse table
void getGrammar()
{
    int termCount, nonTermCount;

    printf("Enter the number of terminals: ");
    scanf("%d", &termCount);
    printf("Enter the terminals: ");
    for (int i = 0; i < termCount; i++)
    {
        scanf(" %c", &terminals[i]);
    }

    printf("Enter the number of non-terminals: ");
    scanf("%d", &nonTermCount);
    printf("Enter the non-terminals: ");
    for (int i = 0; i < nonTermCount; i++)
    {
        scanf(" %c", &nonTerminals[i]);
    }

    printf("Enter the number of productions: ");
    scanf("%d", &productionCount);
    getchar(); // Consume newline

    printf("Enter the productions (use 'e' for epsilon):\n");
    for (int i = 0; i < productionCount; i++)
    {
        production[i] = (char *)malloc(50 * sizeof(char));
        fgets(production[i], 50, stdin);
        production[i][strcspn(production[i], "\n")] = 0; // Remove newline
    }

    // Initialize parse table (for demonstration, you need to define rules to fill it)
    // Set default to 0 (no rule)
    for (int i = 0; i < nonTermCount; i++)
    {
        for (int j = 0; j < termCount; j++)
        {
            parseTable[i][j] = 0;
        }
    }

    // Fill parse table based on the grammar provided
    // For S
    parseTable[0][getTerminalIndex('(')] = 1; // S -> (L)
    parseTable[0][getTerminalIndex('a')] = 2; // S -> a

    // For L
    parseTable[1][getTerminalIndex('(')] = 3; // L -> SA
    parseTable[1][getTerminalIndex('a')] = 3; // L -> SA

    // For A
    parseTable[2][getTerminalIndex(',')] = 4; // A -> ,SA
    parseTable[2][getTerminalIndex(')')] = 5; // A -> e (epsilon)
}

int main()
{
    char input[50];

    // Get grammar from user and build parse table
    getGrammar();

    // Input the string to parse
    printf("Enter the input string (terminated by $): ");
    scanf("%s", input);

    // Call predictive parsing function
    predictiveParsing(input);

    return 0;
}

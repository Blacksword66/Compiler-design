#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100
#define MAX_INPUT_LENGTH 256

void identifyTokens(char *input) {
    char token[MAX_TOKEN_LENGTH];
    int i = 0, j = 0;

    while (input[i] != '\0') {
        // Skip whitespaces
        if (isspace(input[i])) {
            i++;
            continue;
        }

        // If the character is an alphabet or underscore (part of an identifier)
        if (isalpha(input[i]) || input[i] == '_') {
            while (isalpha(input[i]) || isdigit(input[i]) || input[i] == '_') {
                token[j++] = input[i++];
            }
            token[j] = '\0';
            printf("Identifier: %s\n", token);
            j = 0;
        }

        // If the character is a digit (part of a number)
        else if (isdigit(input[i])) {
            while (isdigit(input[i])) {
                token[j++] = input[i++];
            }
            token[j] = '\0';
            printf("Number: %s\n", token);
            j = 0;
        }

        // If the character is an operator
        else if (strchr("+-*/=", input[i])) {
            token[j++] = input[i++];
            token[j] = '\0';
            printf("Operator: %s\n", token);
            j = 0;
        }

        // If the character is a special symbol (like ;, (), etc.)
        else if (strchr(";(),{}", input[i])) {
            token[j++] = input[i++];
            token[j] = '\0';
            printf("Special Symbol: %s\n", token);
            j = 0;
        }

        // Move to next character
        else {
            i++;
        }
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];

    printf("Enter a statement: ");
    fgets(input, MAX_INPUT_LENGTH, stdin);

    // Remove newline character from input if present
    input[strcspn(input, "\n")] = 0;

    printf("Input statement: %s\n", input);
    identifyTokens(input);

    return 0;
}

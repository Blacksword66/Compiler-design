#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100
#define MAX_INPUT_LENGTH 256

const char *keywords[] = {"int", "return", "scanf", "if", "else", "while", "for", "do", "switch", "case", "default", "break", "continue", "void", "char", "float", "double", "long", "short", "unsigned", "signed", "static", "struct", "union", "enum", "typedef", "const", "sizeof", "volatile", "extern", "register", "auto", "goto", "include", "define", NULL};

int isKeyword(const char *token)
{
    for (int i = 0; keywords[i] != NULL; i++)
    {
        if (strcmp(token, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void printOperatorName(char c, FILE *output)
{
    fprintf(output, "Operator, ");
    switch (c)
    {
    case '+':
        printf("Addition: %c\n\n", c);
        break;
    case '-':
        printf("Subtraction: %c\n\n", c);
        break;
    case '*':
        printf("Multiplication: %c\n\n", c);
        break;
    case '/':
        printf("Division: %c\n\n", c);
        break;
    case '=':
        printf("Assignment: %c\n\n", c);
        break;
    case '<':
        printf("Less than: %c\n\n", c);
        break;
    case '>':
        printf("Greater than: %c\n\n", c);
        break;
    case '&':
        printf("Logical AND: %c\n\n", c);
        break;
    case '|':
        printf("Logical OR: %c\n\n", c);
        break;
    case '!':
        printf("Logical NOT: %c\n\n", c);
        break;
    case '^':
        printf("Bitwise XOR: %c\n\n", c);
        break;
    case '%':
        printf("Modulus: %c\n\n", c);
        break;
    case '(':
        printf("Left parenthesis: %c\n\n", c);
        break;
    case ')':
        printf("Right parenthesis: %c\n\n", c);
        break;
    case '{':
        printf("Left curly brace: %c\n\n", c);
        break;
    case '}':
        printf("Right curly brace: %c\n\n", c);
        break;
    case '[':
        printf("Left square bracket: %c\n\n", c);
        break;
    case ']':
        printf("Right square bracket: %c\n\n", c);
        break;
    case ',':
        printf("Comma: %c\n\n", c);
        break;
    case ';':
        printf("Semicolon: %c\n\n", c);
        break;
    default:
        break;
    }
}

void identifyTokens(char *input, FILE *output)
{
    char token[MAX_TOKEN_LENGTH];
    int i = 0, j = 0;

    while (input[i] != '\0')
    {
        if (isspace(input[i]))
        {
            i++;
            continue;
        }

        if (isalpha(input[i]) || input[i] == '_')
        {
            while (isalpha(input[i]) || isdigit(input[i]) || input[i] == '_')
            {
                token[j++] = input[i++];
            }
            token[j] = '\0';

            if (isKeyword(token))
            {
                fprintf(output, "Keyword: %s\n\n", token);
            }
            else
            {
                fprintf(output, "Identifier: %s\n\n", token);
            }
            j = 0;
        }

        else if (isdigit(input[i]))
        {
            while (isdigit(input[i]))
            {
                token[j++] = input[i++];
            }
            token[j] = '\0';
            fprintf(output, "Number: %s\n\n", token);
            j = 0;
        }

        else if (strchr("+-*/=<>!&|^%", input[i]))
        {
            token[j++] = input[i++];
            if ((token[0] == '=' || token[0] == '!' || token[0] == '<' || token[0] == '>') && input[i] == '=')
            {
                token[j++] = input[i++];
            }
            else if ((token[0] == '&' || token[0] == '|') && input[i] == token[0])
            {
                token[j++] = input[i++];
            }
            token[j] = '\0';
            fprintf(output, "Operator: %s\n\n", token);
            printOperatorName(token[0], output);
            j = 0;
        }

        else if (strchr("(){}[];,", input[i]))
        {
            token[j++] = input[i++];
            token[j] = '\0';
            printOperatorName(token[0], output);
            j = 0;
        }

        else
        {
            i++;
        }
    }
}

int main()
{
    FILE *inputFile, *tokens;

    inputFile = fopen("input.txt", "r");
    if (inputFile == NULL)
    {
        printf("Error: Could not open input file.\n");
        return 1;
    }

    tokens = fopen("tokens.txt", "w");
    if (tokens == NULL)
    {
        printf("Error: Could not open output file.\n");
        fclose(inputFile);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), inputFile))
    {
        identifyTokens(line, tokens);
    }
    // Close both files
    fclose(inputFile);
    fclose(tokens);

    printf("Data successfully written to tokens.txt\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1000
#define MAX_TOKENS 5
#define MAX_VARS 100

typedef struct
{
    char name[50];
} Variable;

Variable symbol_table[MAX_VARS];
int var_count = 0;

int is_number(const char *str)
{
    if (str[0] == '-' || str[0] == '+')
        str++;
    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

int add_variable(const char *name)
{
    for (int i = 0; i < var_count; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
        {
            return i; // Variable already exists
        }
    }
    strcpy(symbol_table[var_count].name, name);
    return var_count++;
}

void generate_mnemonics(FILE *output, const char *dest, const char *src1, const char *op, const char *src2)
{
    // Load the first operand into a register (R1)
    if (is_number(src1))
    {
        fprintf(output, "MOV %s, R1\n", src1);
    }
    else
    {
        fprintf(output, "MOV %s, R1\n", src1);
    }

    // Perform the operation with the second operand if it exists
    if (strcmp(op, "") != 0)
    {
        if (is_number(src2))
        {
            fprintf(output, "%s R1, %s\n", strcmp(op, "+") == 0 ? "ADD" : strcmp(op, "-") == 0 ? "SUB"
                                                                      : strcmp(op, "*") == 0   ? "MUL"
                                                                                               : "DIV",
                    src2);
        }
        else
        {
            fprintf(output, "MOV %s, R2\n", src2);
            fprintf(output, "%s R1, R2\n", strcmp(op, "+") == 0 ? "ADD" : strcmp(op, "-") == 0 ? "SUB"
                                                                      : strcmp(op, "*") == 0   ? "MUL"
                                                                                               : "DIV");
        }
    }

    // Store the result back into the destination variable
    fprintf(output, "MOV R1, %s\n", dest);
}

int main()
{
    FILE *input = fopen("input.tac", "r");
    FILE *output = fopen("output.mnemonics", "w");

    if (!input || !output)
    {
        printf("Error opening files!\n");
        return 1;
    }

    char line[MAX_LINE];
    char *tokens[MAX_TOKENS]; // Temporary adjustment for safety

    while (fgets(line, MAX_LINE, input))
    {
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';
        if (strlen(line) == 0)
            continue;

        // Tokenize the line
        char *token = strtok(line, " ");
        int token_count = 0;
        while (token && token_count < MAX_TOKENS)
        {
            tokens[token_count++] = token;
            token = strtok(NULL, " ");
        }

        // Pad the tokens if there are fewer than 4
        while (token_count < MAX_TOKENS)
            tokens[token_count++] = "";

        // Process arithmetic or assignment (e.g., t1 = a + 2)
        if (strcmp(tokens[1], "=") == 0)
        {
            const char *src2 = (token_count > 3) ? tokens[3] : ""; // Safe access to tokens[3]
            generate_mnemonics(output, tokens[0], tokens[2], tokens[3], src2);
        }
    }

    fclose(input);
    fclose(output);
    return 0;
}
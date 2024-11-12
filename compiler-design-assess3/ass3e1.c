#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_RULES 20
#define MAX_SYMBOLS 20
#define MAX_PROD_LEN 20
#define MAX_FIRST 20
#define MAX_FOLLOW 20

typedef struct
{
    char lhs;
    char rhs[10][10];
    int num_rhs;
} Production;

typedef struct
{
    char symbol;
    char first[MAX_FIRST];
    int first_count;
    char follow[MAX_FOLLOW];
    int follow_count;
} SymbolInfo;

int is_terminal(char c);
int find_symbol(SymbolInfo symbols[], int num_symbols, char c);
void compute_FIRST(Production grammar[], int n, SymbolInfo symbols[], int num_symbols);
void compute_FOLLOW(Production grammar[], int n, SymbolInfo symbols[], int num_symbols, char start_symbol);
void construct_parse_table(Production grammar[], int n, SymbolInfo symbols[], int num_symbols, char parse_table[][MAX_SYMBOLS][MAX_PROD_LEN], int num_terminals, char terminals[], int num_nonterminals, char nonterminals[]);

int is_terminal(char c)
{
    if (c == 'e')
        return 0;
    if (c >= 'A' && c <= 'Z')
        return 0;
    return 1;
}

int find_symbol(SymbolInfo symbols[], int num_symbols, char c)
{
    for (int i = 0; i < num_symbols; i++)
    {
        if (symbols[i].symbol == c)
            return i;
    }
    return -1;
}

int main()
{
    Production grammar[MAX_RULES];
    int n;

    printf("Enter the number of productions: ");
    scanf("%d", &n);

    printf("Enter the productions (e.g., S->AB | b):\n");
    for (int i = 0; i < n; i++)
    {
        char input[100];
        printf("Production %d: ", i + 1);
        scanf(" %s", input);
        char *lhs = strtok(input, "->");
        char *rhs = strtok(NULL, "");
        if (lhs == NULL || rhs == NULL)
        {
            printf("Invalid production format. Use A->B|C format.\n");
            exit(1);
        }
        grammar[i].lhs = lhs[0];
        grammar[i].num_rhs = 0;
        char *prod = strtok(rhs, "|");
        while (prod != NULL && grammar[i].num_rhs < 10)
        {
            strcpy(grammar[i].rhs[grammar[i].num_rhs], prod);
            grammar[i].num_rhs++;
            prod = strtok(NULL, "|");
        }
    }

    SymbolInfo symbols[MAX_SYMBOLS];
    int num_symbols = 0;

    for (int i = 0; i < n; i++)
    {
        char lhs = grammar[i].lhs;
        if (find_symbol(symbols, num_symbols, lhs) == -1)
        {
            symbols[num_symbols].symbol = lhs;
            symbols[num_symbols].first_count = 0;
            symbols[num_symbols].follow_count = 0;
            symbols[num_symbols].first[0] = '\0';
            symbols[num_symbols].follow[0] = '\0';
            num_symbols++;
        }
    }

    char terminals[MAX_SYMBOLS];
    int num_terminals = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < grammar[i].num_rhs; j++)
        {
            char *prod = grammar[i].rhs[j];
            for (int k = 0; k < strlen(prod); k++)
            {
                char c = prod[k];
                if (is_terminal(c) && c != 'e')
                {
                    if (strchr(terminals, c) == NULL && num_terminals < MAX_SYMBOLS)
                    {
                        terminals[num_terminals++] = c;
                    }
                }
            }
        }
    }

    compute_FIRST(grammar, n, symbols, num_symbols);
    char start_symbol = grammar[0].lhs;
    compute_FOLLOW(grammar, n, symbols, num_symbols, start_symbol);

    char nonterminals[MAX_SYMBOLS];
    int num_nonterminals = num_symbols;
    for (int i = 0; i < num_nonterminals; i++)
    {
        nonterminals[i] = symbols[i].symbol;
    }

    char parse_table_non_null[MAX_SYMBOLS][MAX_SYMBOLS][MAX_PROD_LEN];
    memset(parse_table_non_null, 0, sizeof(parse_table_non_null));

    construct_parse_table(grammar, n, symbols, num_symbols, parse_table_non_null, num_terminals, terminals, num_nonterminals, nonterminals);

    printf("\nPredictive Parse Table:\n\t");
    for (int i = 0; i < num_terminals; i++)
    {
        printf("%c\t", terminals[i]);
    }
    printf("$\t\n");

    char all_terminals[MAX_SYMBOLS];
    int total_terminals = num_terminals;
    all_terminals[total_terminals++] = '$';

    for (int i = 0; i < num_nonterminals; i++)
    {
        printf("%c\t", nonterminals[i]);
        for (int j = 0; j < num_terminals; j++)
        {
            if (parse_table_non_null[i][j][0] != '\0')
            {
                printf("%s\t", parse_table_non_null[i][j]);
            }
            else
            {
                printf("\t");
            }
        }

        if (parse_table_non_null[i][num_terminals][0] != '\0')
        {
            printf("%s\t", parse_table_non_null[i][num_terminals]);
        }
        else
        {
            printf("\t");
        }
        printf("\n");
    }

    return 0;
}

void compute_FIRST(Production grammar[], int n, SymbolInfo symbols[], int num_symbols)
{
    int changed = 1;
    while (changed)
    {
        changed = 0;
        for (int i = 0; i < n; i++)
        {
            char A = grammar[i].lhs;
            int A_index = find_symbol(symbols, num_symbols, A);
            for (int j = 0; j < grammar[i].num_rhs; j++)
            {
                char *prod = grammar[i].rhs[j];
                int k = 0;
                int epsilon_in_prev = 1;
                while (prod[k] != '\0' && epsilon_in_prev)
                {
                    char symbol = prod[k];
                    if (symbol == 'e')
                    {

                        if (strchr(symbols[A_index].first, 'e') == NULL && symbols[A_index].first_count < MAX_FIRST - 1)
                        {
                            symbols[A_index].first[symbols[A_index].first_count++] = 'e';
                            symbols[A_index].first[symbols[A_index].first_count] = '\0';
                            changed = 1;
                        }
                        epsilon_in_prev = 0;
                    }
                    else if (is_terminal(symbol))
                    {

                        if (strchr(symbols[A_index].first, symbol) == NULL && symbols[A_index].first_count < MAX_FIRST - 1)
                        {
                            symbols[A_index].first[symbols[A_index].first_count++] = symbol;
                            symbols[A_index].first[symbols[A_index].first_count] = '\0';
                            changed = 1;
                        }
                        epsilon_in_prev = 0;
                    }
                    else
                    {
                        int B_index = find_symbol(symbols, num_symbols, symbol);
                        if (B_index == -1)
                        {
                            printf("Error: Symbol %c not found in FIRST computation.\n", symbol);
                            exit(1);
                        }

                        for (int m = 0; m < symbols[B_index].first_count; m++)
                        {
                            char first_sym = symbols[B_index].first[m];
                            if (first_sym != 'e' && strchr(symbols[A_index].first, first_sym) == NULL)
                            {
                                symbols[A_index].first[symbols[A_index].first_count++] = first_sym;
                                symbols[A_index].first[symbols[A_index].first_count] = '\0';
                                changed = 1;
                            }
                        }

                        int can_derive_epsilon = 0;
                        for (int m = 0; m < symbols[B_index].first_count; m++)
                        {
                            if (symbols[B_index].first[m] == 'e')
                            {
                                can_derive_epsilon = 1;
                                break;
                            }
                        }
                        epsilon_in_prev = can_derive_epsilon;
                    }
                }
                if (epsilon_in_prev)
                {
                    if (strchr(symbols[A_index].first, 'e') == NULL && symbols[A_index].first_count < MAX_FIRST - 1)
                    {
                        symbols[A_index].first[symbols[A_index].first_count++] = 'e';
                        symbols[A_index].first[symbols[A_index].first_count] = '\0';
                        changed = 1;
                    }
                }
            }
        }
    }

    printf("\nFIRST sets:\n");
    for (int i = 0; i < num_symbols; i++)
    {
        printf("FIRST(%c) = { ", symbols[i].symbol);
        for (int j = 0; j < symbols[i].first_count; j++)
        {
            printf("%c ", symbols[i].first[j]);
        }
        printf("}\n");
    }
}

void compute_FOLLOW(Production grammar[], int n, SymbolInfo symbols[], int num_symbols, char start_symbol)
{
    int start_index = find_symbol(symbols, num_symbols, start_symbol);
    symbols[start_index].follow[symbols[start_index].follow_count++] = '$';
    symbols[start_index].follow[symbols[start_index].follow_count] = '\0';

    int changed = 1;
    while (changed)
    {
        changed = 0;
        for (int i = 0; i < n; i++)
        {
            char A = grammar[i].lhs;
            int A_index = find_symbol(symbols, num_symbols, A);
            for (int j = 0; j < grammar[i].num_rhs; j++)
            {
                char *prod = grammar[i].rhs[j];
                int len = strlen(prod);
                for (int k = 0; k < len; k++)
                {
                    char B = prod[k];
                    if (!(B >= 'A' && B <= 'Z'))
                        continue;
                    int B_index = find_symbol(symbols, num_symbols, B);
                    if (B_index == -1)
                    {
                        printf("Error: Symbol %c not found in FOLLOW computation.\n", B);
                        exit(1);
                    }

                    char first_beta[MAX_FIRST];
                    int first_beta_count = 0;
                    int can_derive_epsilon = 1;
                    for (int m = k + 1; m < len; m++)
                    {
                        char beta = prod[m];
                        if (beta == 'e')
                        {

                            can_derive_epsilon = 1;
                            break;
                        }
                        if (is_terminal(beta))
                        {
                            if (strchr(first_beta, beta) == NULL && first_beta_count < MAX_FIRST - 1)
                            {
                                first_beta[first_beta_count++] = beta;
                                first_beta[first_beta_count] = '\0';
                            }
                            can_derive_epsilon = 0;
                            break;
                        }
                        else
                        {
                            int beta_index = find_symbol(symbols, num_symbols, beta);
                            if (beta_index == -1)
                            {
                                printf("Error: Symbol %c not found in FIRST(beta) computation.\n", beta);
                                exit(1);
                            }
                            for (int p = 0; p < symbols[beta_index].first_count; p++)
                            {
                                char f = symbols[beta_index].first[p];
                                if (f != 'e' && strchr(first_beta, f) == NULL && first_beta_count < MAX_FIRST - 1)
                                {
                                    first_beta[first_beta_count++] = f;
                                    first_beta[first_beta_count] = '\0';
                                }
                            }

                            int temp = 0;
                            for (int p = 0; p < symbols[beta_index].first_count; p++)
                            {
                                if (symbols[beta_index].first[p] == 'e')
                                {
                                    temp = 1;
                                    break;
                                }
                            }
                            if (!temp)
                            {
                                can_derive_epsilon = 0;
                                break;
                            }
                        }
                    }
                    if (can_derive_epsilon)
                    {
                        for (int p = 0; p < symbols[A_index].follow_count; p++)
                        {
                            char f = symbols[A_index].follow[p];
                            if (strchr(symbols[B_index].follow, f) == NULL && symbols[B_index].follow_count < MAX_FOLLOW - 1)
                            {
                                symbols[B_index].follow[symbols[B_index].follow_count++] = f;
                                symbols[B_index].follow[symbols[B_index].follow_count] = '\0';
                                changed = 1;
                            }
                        }
                    }

                    for (int p = 0; p < first_beta_count; p++)
                    {
                        char f = first_beta[p];
                        if (f != 'e' && strchr(symbols[B_index].follow, f) == NULL && symbols[B_index].follow_count < MAX_FOLLOW - 1)
                        {
                            symbols[B_index].follow[symbols[B_index].follow_count++] = f;
                            symbols[B_index].follow[symbols[B_index].follow_count] = '\0';
                            changed = 1;
                        }
                    }
                }
            }
        }
    }

    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < num_symbols; i++)
    {
        printf("FOLLOW(%c) = { ", symbols[i].symbol);
        for (int j = 0; j < symbols[i].follow_count; j++)
        {
            printf("%c ", symbols[i].follow[j]);
        }
        printf("}\n");
    }
}

void construct_parse_table(Production grammar[], int n, SymbolInfo symbols[], int num_symbols, char parse_table[][MAX_SYMBOLS][MAX_PROD_LEN], int num_terminals, char terminals[], int num_nonterminals, char nonterminals[])
{

    for (int i = 0; i < num_nonterminals; i++)
    {
        for (int j = 0; j < num_terminals; j++)
        {
            parse_table[i][j][0] = '\0';
        }

        parse_table[i][num_terminals][0] = '\0';
    }

    for (int i = 0; i < n; i++)
    {
        char A = grammar[i].lhs;
        int A_index = -1;
        for (int m = 0; m < num_nonterminals; m++)
        {
            if (nonterminals[m] == A)
            {
                A_index = m;
                break;
            }
        }
        if (A_index == -1)
        {
            printf("Error: Non-terminal %c not found.\n", A);
            exit(1);
        }
        for (int j = 0; j < grammar[i].num_rhs; j++)
        {
            char *alpha = grammar[i].rhs[j];
            char first_alpha[MAX_FIRST];
            int first_alpha_count = 0;
            int can_derive_epsilon = 1;
            int k = 0;
            while (alpha[k] != '\0' && can_derive_epsilon)
            {
                char symbol = alpha[k];
                if (symbol == 'e')
                {

                    if (strchr(first_alpha, 'e') == NULL && first_alpha_count < MAX_FIRST - 1)
                    {
                        first_alpha[first_alpha_count++] = 'e';
                        first_alpha[first_alpha_count] = '\0';
                    }
                    can_derive_epsilon = 0;
                }
                else if (is_terminal(symbol))
                {
                    if (strchr(first_alpha, symbol) == NULL && first_alpha_count < MAX_FIRST - 1)
                    {
                        first_alpha[first_alpha_count++] = symbol;
                        first_alpha[first_alpha_count] = '\0';
                    }
                    can_derive_epsilon = 0;
                }
                else
                {
                    int B_index = find_symbol(symbols, num_symbols, symbol);
                    if (B_index == -1)
                    {
                        printf("Error: Symbol %c not found in FIRST(alpha) computation.\n", symbol);
                        exit(1);
                    }
                    for (int p = 0; p < symbols[B_index].first_count; p++)
                    {
                        char f = symbols[B_index].first[p];
                        if (f != 'e' && strchr(first_alpha, f) == NULL && first_alpha_count < MAX_FIRST - 1)
                        {
                            first_alpha[first_alpha_count++] = f;
                            first_alpha[first_alpha_count] = '\0';
                        }
                    }

                    int temp = 0;
                    for (int p = 0; p < symbols[B_index].first_count; p++)
                    {
                        if (symbols[B_index].first[p] == 'e')
                        {
                            temp = 1;
                            break;
                        }
                    }
                    if (!temp)
                    {
                        can_derive_epsilon = 0;
                    }
                }
                k++;
            }
            if (can_derive_epsilon)
            {
                for (int p = 0; p < symbols[A_index].follow_count; p++)
                {
                    char f = symbols[A_index].follow[p];
                    if (strchr(first_alpha, f) == NULL && first_alpha_count < MAX_FIRST - 1)
                    {
                        first_alpha[first_alpha_count++] = f;
                        first_alpha[first_alpha_count] = '\0';
                    }
                }
            }

            for (int p = 0; p < first_alpha_count; p++)
            {
                char terminal = first_alpha[p];
                int term_index = -1;
                for (int t = 0; t < num_terminals; t++)
                {
                    if (terminals[t] == terminal)
                    {
                        term_index = t;
                        break;
                    }
                }
                if (terminal == '$')
                {
                    term_index = num_terminals;
                }
                if (term_index != -1)
                {

                    if (parse_table[A_index][term_index][0] != '\0')
                    {
                        printf("Error: Conflict in parse table for Non-terminal %c and Terminal %c.\n", A, terminal);
                        exit(1);
                    }
                    strcpy(parse_table[A_index][term_index], alpha);
                }
            }
        }
    }

    for (int i = 0; i < num_nonterminals; i++)
    {
        for (int j = 0; j < num_terminals; j++)
        {
            if (parse_table[i][j][0] == '\0')
            {

                if (strchr(symbols[i].first, 'e') != NULL)
                {

                    for (int p = 0; p < symbols[i].follow_count; p++)
                    {
                        char f = symbols[i].follow[p];

                        int term_index = -1;
                        for (int t = 0; t < num_terminals; t++)
                        {
                            if (terminals[t] == f)
                            {
                                term_index = t;
                                break;
                            }
                        }
                        if (f == '$')
                        {
                            term_index = num_terminals;
                        }
                        if (term_index != -1 && parse_table[i][term_index][0] == '\0')
                        {
                            strcpy(parse_table[i][term_index], "e");
                        }
                    }
                }
            }
        }
    }
}

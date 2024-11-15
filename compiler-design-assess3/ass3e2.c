#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 100
#define MAX_PRODUCTIONS 10
#define MAX_TERMINALS 20
#define MAX_NONTERMINALS 20

typedef struct
{
    char lhs;
    char rhs[MAX_PRODUCTIONS][MAX_SYMBOLS];
    int count;
} Production;

typedef struct
{
    char terminal;
    char production[MAX_SYMBOLS];
} TerminalProduction;

typedef struct
{
    char nonTerminal;
    TerminalProduction entries[MAX_TERMINALS];
    int count;
} ParseTableEntry;

Production productions[MAX_NONTERMINALS];
int productionCount = 0;
char terminals[MAX_TERMINALS];
int terminalCount = 0;
char nonTerminals[MAX_NONTERMINALS];
int nonTerminalCount = 0;
char firstSets[MAX_NONTERMINALS][MAX_SYMBOLS][MAX_SYMBOLS];
int firstCount[MAX_NONTERMINALS];
char followSets[MAX_NONTERMINALS][MAX_SYMBOLS][MAX_SYMBOLS];
int followCount[MAX_NONTERMINALS];
ParseTableEntry parseTable[MAX_NONTERMINALS];
int parseTableCount = 0;

int isNonTerminalChar(char c)
{
    return isupper(c);
}

int containsString(char set[][MAX_SYMBOLS], int count, char *symbol)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(set[i], symbol) == 0)
            return 1;
    }
    return 0;
}

void addTerminal(char c)
{
    if (!isNonTerminalChar(c) && c != 'e' && c != '$')
    {
        for (int i = 0; i < terminalCount; i++)
        {
            if (terminals[i] == c)
                return;
        }
        terminals[terminalCount++] = c;
    }
}

void addNonTerminal(char c)
{
    for (int i = 0; i < nonTerminalCount; i++)
    {
        if (nonTerminals[i] == c)
            return;
    }
    nonTerminals[nonTerminalCount++] = c;
}

int findProductionIndex(char lhs)
{
    for (int i = 0; i < productionCount; i++)
    {
        if (productions[i].lhs == lhs)
            return i;
    }
    return -1;
}

void addProduction(char lhs, const char *rhs)
{
    int index = findProductionIndex(lhs);
    if (index == -1)
    {
        productions[productionCount].lhs = lhs;
        strcpy(productions[productionCount].rhs[productions[productionCount].count++], rhs);
        addNonTerminal(lhs);
        productionCount++;
    }
    else
    {
        strcpy(productions[index].rhs[productions[index].count++], rhs);
    }
}

void computeFirst(char symbol, char result[][MAX_SYMBOLS], int *resCount)
{
    if (!isNonTerminalChar(symbol))
    {
        char temp[2] = {symbol, '\0'};
        if (!containsString(result, *resCount, temp))
        {
            strcpy(result[*resCount], temp);
            (*resCount)++;
        }
        return;
    }
    int index = findProductionIndex(symbol);
    if (index == -1)
        return;
    for (int i = 0; i < productions[index].count; i++)
    {
        char first = productions[index].rhs[i][0];
        if (first == 'e')
        {
            char temp[] = "e";
            if (!containsString(result, *resCount, temp))
            {
                strcpy(result[*resCount], temp);
                (*resCount)++;
            }
        }
        else if (!isNonTerminalChar(first))
        {
            char temp[2] = {first, '\0'};
            if (!containsString(result, *resCount, temp))
            {
                strcpy(result[*resCount], temp);
                (*resCount)++;
            }
            addTerminal(first);
        }
        else
        {
            computeFirst(first, result, resCount);
        }
    }
}
void computeFirstSets()
{
    for (int i = 0; i < nonTerminalCount; i++)
    {
        firstCount[i] = 0;
        computeFirst(nonTerminals[i], firstSets[i], &firstCount[i]);
    }
}
void computeFollow(char symbol, char result[][MAX_SYMBOLS], int *resCount)
{
    if (symbol == productions[0].lhs)
    {
        char temp[] = "$";
        if (!containsString(result, *resCount, temp))
        {
            strcpy(result[*resCount], temp);
            (*resCount)++;
        }
    }
    for (int i = 0; i < productionCount; i++)
    {
        for (int j = 0; j < productions[i].count; j++)
        {
            char *prod = productions[i].rhs[j];
            for (int k = 0; k < strlen(prod); k++)
            {
                if (prod[k] == symbol)
                {
                    if (k + 1 < strlen(prod))
                    {
                        char next = prod[k + 1];
                        if (next == 'e')
                        {
                            computeFollow(productions[i].lhs, result, resCount);
                        }
                        else if (!isNonTerminalChar(next))
                        {
                            char temp[2] = {next, '\0'};
                            if (!containsString(result, *resCount, temp))
                            {
                                strcpy(result[*resCount], temp);
                                (*resCount)++;
                            }
                        }
                        else
                        {
                            int nextIndex = findProductionIndex(next);
                            if (nextIndex != -1)
                            {
                                for (int f = 0; f < firstCount[nextIndex]; f++)
                                {
                                    if (strcmp(firstSets[nextIndex][f], "e") != 0)
                                    {
                                        if (!containsString(result, *resCount, firstSets[nextIndex][f]))
                                        {
                                            strcpy(result[*resCount], firstSets[nextIndex][f]);
                                            (*resCount)++;
                                        }
                                    }
                                }
                                int contains_e = 0;
                                for (int f = 0; f < firstCount[nextIndex]; f++)
                                {
                                    if (strcmp(firstSets[nextIndex][f], "e") == 0)
                                    {
                                        contains_e = 1;
                                        break;
                                    }
                                }
                                if (contains_e)
                                {
                                    int lhsIndex = findProductionIndex(productions[i].lhs);
                                    if (lhsIndex != -1)
                                    {
                                        for (int f = 0; f < followCount[lhsIndex]; f++)
                                        {
                                            if (!containsString(result, *resCount, followSets[lhsIndex][f]))
                                            {
                                                strcpy(result[*resCount], followSets[lhsIndex][f]);
                                                (*resCount)++;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        int lhsIndex = findProductionIndex(productions[i].lhs);
                        if (lhsIndex != -1)
                        {
                            for (int f = 0; f < followCount[lhsIndex]; f++)
                            {
                                if (!containsString(result, *resCount, followSets[lhsIndex][f]))
                                {
                                    strcpy(result[*resCount], followSets[lhsIndex][f]);
                                    (*resCount)++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void computeFollowSets()
{
    for (int i = 0; i < nonTerminalCount; i++)
    {
        followCount[i] = 0;
    }
    int changed = 1;
    while (changed)
    {
        changed = 0;
        for (int i = 0; i < nonTerminalCount; i++)
        {
            char current = nonTerminals[i];
            char tempSet[MAX_SYMBOLS][MAX_SYMBOLS];
            int tempCount = 0;
            computeFollow(current, tempSet, &tempCount);
            for (int j = 0; j < tempCount; j++)
            {
                if (!containsString(followSets[i], followCount[i], tempSet[j]))
                {
                    strcpy(followSets[i][followCount[i]++], tempSet[j]);
                    changed = 1;
                }
            }
        }
    }
}

void computeParseTable()
{
    for (int i = 0; i < nonTerminalCount; i++)
    {
        parseTable[i].nonTerminal = nonTerminals[i];
        parseTable[i].count = 0;
    }
    for (int i = 0; i < productionCount; i++)
    {
        char lhs = productions[i].lhs;
        int lhsIndex = -1;
        for (int nt = 0; nt < nonTerminalCount; nt++)
        {
            if (nonTerminals[nt] == lhs)
            {
                lhsIndex = nt;
                break;
            }
        }
        if (lhsIndex == -1)
            continue;
        for (int p = 0; p < productions[i].count; p++)
        {
            char *prod = productions[i].rhs[p];
            if (prod[0] == 'e')
            {
                for (int f = 0; f < followCount[lhsIndex]; f++)
                {
                    parseTable[lhsIndex].entries[parseTable[lhsIndex].count].terminal = followSets[lhsIndex][f][0];
                    strcpy(parseTable[lhsIndex].entries[parseTable[lhsIndex].count].production, "e");
                    parseTable[lhsIndex].count++;
                }
            }
            else
            {
                char first = prod[0];
                if (!isNonTerminalChar(first))
                {
                    parseTable[lhsIndex].entries[parseTable[lhsIndex].count].terminal = first;
                    strcpy(parseTable[lhsIndex].entries[parseTable[lhsIndex].count].production, prod);
                    parseTable[lhsIndex].count++;
                    addTerminal(first);
                }
                else
                {
                    int firstIndex = findProductionIndex(first);
                    if (firstIndex != -1)
                    {
                        for (int f = 0; f < firstCount[firstIndex]; f++)
                        {
                            if (strcmp(firstSets[firstIndex][f], "e") != 0)
                            {
                                parseTable[lhsIndex].entries[parseTable[lhsIndex].count].terminal = firstSets[firstIndex][f][0];
                                strcpy(parseTable[lhsIndex].entries[parseTable[lhsIndex].count].production, prod);
                                parseTable[lhsIndex].count++;
                            }
                        }
                        int contains_e = 0;
                        for (int f = 0; f < firstCount[firstIndex]; f++)
                        {
                            if (strcmp(firstSets[firstIndex][f], "e") == 0)
                            {
                                contains_e = 1;
                                break;
                            }
                        }
                        if (contains_e)
                        {
                            for (int f = 0; f < followCount[lhsIndex]; f++)
                            {
                                parseTable[lhsIndex].entries[parseTable[lhsIndex].count].terminal = followSets[lhsIndex][f][0];
                                strcpy(parseTable[lhsIndex].entries[parseTable[lhsIndex].count].production, prod);
                                parseTable[lhsIndex].count++;
                            }
                        }
                    }
                }
            }
        }
    }
}

void displayParseTable()
{
    printf("\nParse Table:\n");
    printf("NT\tTerminal\tProduction\n");
    for (int i = 0; i < nonTerminalCount; i++)
    {
        for (int j = 0; j < parseTable[i].count; j++)
        {
            printf("%c\t%c\t\t%c->%s\n", parseTable[i].nonTerminal, parseTable[i].entries[j].terminal, parseTable[i].nonTerminal, parseTable[i].entries[j].production);
        }
    }
}

void predictiveParse(char input_string[], ParseTableEntry parse_table[], int rule_count)
{
    char stack[MAX_SYMBOLS];
    int top = 0;
    stack[top++] = '$';
    // stack[top++] = 'E';
    //
    stack[top++] = 'S';
    strcat(input_string, "$");
    int ptr = 0;
    printf("\nStack\tInput\t\tAction\n");
    while (top > 0)
    {
        stack[top] = '\0';
        if (strlen(stack) > 3)
        {
            printf("%s\t%s\t\t", stack, &input_string[ptr]);
        }
        else
        {
            printf("%s\t\t%s\t\t", stack, &input_string[ptr]);
        }
        char topSymbol = stack[top - 1];
        char current = input_string[ptr];
        if (topSymbol == current)
        {
            printf("Match\n");
            top--;
            ptr++;
            if (topSymbol == '$' && current == '$')
            {
                printf("Parsing successful\n");
                break;
            }
        }
        else
        {
            int found = 0;
            int ntIndex = -1;
            for (int i = 0; i < nonTerminalCount; i++)
            {
                if (parse_table[i].nonTerminal == topSymbol)
                {
                    ntIndex = i;
                    break;
                }
            }
            if (ntIndex != -1)
            {
                for (int j = 0; j < parse_table[ntIndex].count; j++)
                {
                    if (parse_table[ntIndex].entries[j].terminal == current)
                    {
                        printf("Output %c->%s\n", topSymbol, parse_table[ntIndex].entries[j].production);
                        top--;
                        if (strcmp(parse_table[ntIndex].entries[j].production, "e") != 0)
                        {
                            int len = strlen(parse_table[ntIndex].entries[j].production);
                            for (int k = len - 1; k >= 0; k--)
                                stack[top++] = parse_table[ntIndex].entries[j].production[k];
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found)
                {
                    printf("Rejected\n");
                    break;
                }
            }
            else
            {
                printf("Rejected\n");
                break;
            }
        }
    }
}

int main()
{
    addProduction('S', "(L)");
    addProduction('S', "a");
    addProduction('L', "SA");
    addProduction('A', ",SA");
    addProduction('A', "e");

    computeFirstSets();

    computeFollowSets();

    computeParseTable();

    displayParseTable();

    char input[MAX_SYMBOLS];
    printf("\nEnter input string: ");
    scanf("%s", input);

    predictiveParse(input, parseTable, productionCount);

    return 0;
}
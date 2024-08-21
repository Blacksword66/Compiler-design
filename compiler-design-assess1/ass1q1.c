#include <stdio.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 26
#define MAX_LABEL_LENGTH 10

// Structure to represent DFA
typedef struct
{
    int transition[MAX_STATES][MAX_SYMBOLS];        // Transition table
    int numStates;                                  // Number of states
    int startState;                                 // Start state
    int acceptStates[MAX_STATES];                   // Accept states
    char stateLabels[MAX_STATES][MAX_LABEL_LENGTH]; // State labels
    char symbols[MAX_SYMBOLS];                      // Input symbols
} DFA;

// Function to initialize DFA
void initializeDFA(DFA *dfa)
{
    memset(dfa->transition, -1, sizeof(dfa->transition));    // Initialize all transitions to -1
    memset(dfa->acceptStates, 0, sizeof(dfa->acceptStates)); // Initialize all accept states to 0 (not accepting)
}

// Function to find the index of a state by its label
int findStateIndex(DFA *dfa, char *label)
{
    for (int i = 0; i < dfa->numStates; i++)
    {
        if (strcmp(dfa->stateLabels[i], label) == 0)
        {
            return i;
        }
    }
    return -1; // State not found
}

// Function to find the index of a symbol
int findSymbolIndex(DFA *dfa, char symbol)
{
    for (int i = 0; i < strlen(dfa->symbols); i++)
    {
        if (dfa->symbols[i] == symbol)
        {
            return i;
        }
    }
    return -1; // Symbol not found
}

// Function to create DFA based on user input
void createDFA(DFA *dfa)
{
    printf("Enter the number of states: ");
    scanf("%d", &(dfa->numStates));

    printf("Enter the state labels:\n");
    for (int i = 0; i < dfa->numStates; i++)
    {
        printf("State %d: ", i + 1);
        scanf("%s", dfa->stateLabels[i]);
    }

    printf("Enter the number of input symbols: ");
    int numSymbols;
    scanf("%d", &numSymbols);

    printf("Enter the symbols:\n");
    for (int i = 0; i < numSymbols; i++)
    {
        printf("Symbol %d: ", i + 1);
        scanf(" %c", &(dfa->symbols[i]));
    }

    char startLabel[MAX_LABEL_LENGTH];
    printf("Enter the start state label: ");
    scanf("%s", startLabel);
    dfa->startState = findStateIndex(dfa, startLabel);

    int numAcceptStates;
    printf("Enter the number of accept states: ");
    scanf("%d", &numAcceptStates);

    printf("Enter the final state labels:\n");
    for (int i = 0; i < numAcceptStates; i++)
    {
        char acceptLabel[MAX_LABEL_LENGTH];
        scanf("%s", acceptLabel);
        int index = findStateIndex(dfa, acceptLabel);
        if (index != -1)
        {
            dfa->acceptStates[index] = 1; // Mark state as accepting
        }
    }

    int numTransitions = numSymbols * dfa->numStates;
    // printf("Enter the number of transitions: ");
    // scanf("%d", &numTransitions);

    printf("Enter transitions:\n");
    // for (int i = 0; i < numTransitions; i++)
    // {
    //     char currentState[MAX_LABEL_LENGTH], nextState[MAX_LABEL_LENGTH];
    //     char symbol;
    //     scanf("%s %c %s", currentState, &symbol, nextState);

    //     int currentIndex = findStateIndex(dfa, currentState);
    //     int symbolIndex = findSymbolIndex(dfa, symbol);
    //     int nextIndex = findStateIndex(dfa, nextState);

    //     if (currentIndex != -1 && symbolIndex != -1 && nextIndex != -1)
    //     {
    //         dfa->transition[currentIndex][symbolIndex] = nextIndex;
    //     }
    // }
    for (int i = 0; i < dfa->numStates; i++)
    {
        for (int j = 0; j < numSymbols; j++)
        {
            char currentState[MAX_LABEL_LENGTH], nextState[MAX_LABEL_LENGTH];
            char symbol;
            printf("D[%s, %c] = ", dfa->stateLabels[i], dfa->symbols[j]);
            scanf("%s", nextState);
            strcpy(currentState, dfa->stateLabels[i]);
            symbol = dfa->symbols[j];
            int currentIndex = i;
            int symbolIndex = j;
            int nextIndex = findStateIndex(dfa, nextState);
            if (currentIndex != -1 && symbolIndex != -1 && nextIndex != -1)
            {
                dfa->transition[currentIndex][symbolIndex] = nextIndex;
            }
            else
            {
                dfa->transition[currentIndex][symbolIndex] = -2;
            }
        }
    }

    for (int i = 0; i <= numSymbols; i++)
    {
        if (i == 0)
        {
            printf("\t");
        }
        else
        {
            printf("%c\t", dfa->symbols[i - 1]);
        }
    }
    printf("\n");

    for (int i = 0; i <= dfa->numStates; i++)
    {
        printf("%s\t", dfa->stateLabels[i]);
        for (int j = 0; j < numSymbols; j++)
        {
            int nextState = dfa->transition[i][j];
            if (nextState != -2)
            {
                printf("%s\t", dfa->stateLabels[nextState]);
            }
            else
            {
                printf("-\t");
            }
        }
        printf("\n");
    }
}

// Function to check if a string is accepted by the DFA and print transitions
int isAccepted(DFA *dfa, const char *input)
{
    int currentState = dfa->startState;

    printf("Transition path: %s", dfa->stateLabels[currentState]);

    for (int i = 0; i < strlen(input); i++)
    {
        char symbol = input[i];
        int symbolIndex = findSymbolIndex(dfa, symbol);
        int nextState = dfa->transition[currentState][symbolIndex];

        if (nextState == -1)
        {
            printf("\nInvalid transition on symbol '%c'.\n", symbol);
            return 0; // Transition not defined, string is rejected
        }

        currentState = nextState;
        printf(" -> '%c' -> %s", symbol, dfa->stateLabels[currentState]);
    }

    printf("\n");
    return dfa->acceptStates[currentState];
}

int main()
{
    DFA dfa;
    initializeDFA(&dfa);
    createDFA(&dfa);

    char input[100];
    printf("Enter a string to check: ");
    scanf("%s", input);

    if (isAccepted(&dfa, input))
    {
        printf("The string is accepted by the DFA.\n");
    }
    else
    {
        printf("The string is rejected by the DFA.\n");
    }

    return 0;
}

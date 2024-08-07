#include <stdio.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 26

// Structure to represent DFA
typedef struct {
    int transition[MAX_STATES][MAX_SYMBOLS]; // Transition table
    int numStates; // Number of states
    int startState; // Start state
    int acceptStates[MAX_STATES]; // Accept states
} DFA;

// Function to initialize DFA
void initializeDFA(DFA *dfa) {
    memset(dfa->transition, -1, sizeof(dfa->transition)); // Initialize all transitions to -1
    memset(dfa->acceptStates, 0, sizeof(dfa->acceptStates)); // Initialize all accept states to 0 (not accepting)
}

// Function to create DFA based on user input
void createDFA(DFA *dfa) {
    printf("Enter the number of states: ");
    scanf("%d", &(dfa->numStates));
    
    printf("Enter the start state (0 to %d): ", dfa->numStates - 1);
    scanf("%d", &(dfa->startState));

    int numAcceptStates;
    printf("Enter the number of accept states: ");
    scanf("%d", &numAcceptStates);

    printf("Enter the accept states: ");
    for (int i = 0; i < numAcceptStates; i++) {
        int state;
        scanf("%d", &state);
        dfa->acceptStates[state] = 1; // Mark state as accepting
    }

    int numTransitions;
    printf("Enter the number of transitions: ");
    scanf("%d", &numTransitions);

    printf("Enter transitions in the format 'current_state symbol next_state':\n");
    for (int i = 0; i < numTransitions; i++) {
        int currentState, nextState;
        char symbol;
        scanf("%d %c %d", &currentState, &symbol, &nextState);
        dfa->transition[currentState][symbol - 'a'] = nextState;
    }
}

// Function to check if a string is accepted by the DFA and print transitions
int isAccepted(DFA *dfa, const char *input) {
    int currentState = dfa->startState;
    
    printf("Transition path: q%d", currentState);
    
    for (int i = 0; i < strlen(input); i++) {
        char symbol = input[i];
        int nextState = dfa->transition[currentState][symbol - 'a'];
        
        if (nextState == -1) {
            printf("\nInvalid transition on symbol '%c'.\n", symbol);
            return 0; // Transition not defined, string is rejected
        }

        currentState = nextState;
        printf(" -> '%c' -> q%d", symbol, currentState);
    }
    
    printf("\n");
    return dfa->acceptStates[currentState];
}

int main() {
    DFA dfa;
    initializeDFA(&dfa);
    createDFA(&dfa);

    char input[100];
    printf("Enter a string to check: ");
    scanf("%s", input);

    if (isAccepted(&dfa, input)) {
        printf("The string is accepted by the DFA.\n");
    } else {
        printf("The string is rejected by the DFA.\n");
    }

    return 0;
}

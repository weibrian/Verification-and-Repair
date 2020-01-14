/** @file examples.c
 *  @brief Example state machines and patterns
 *  @author Brian Wei
 *
 *  Detailed documentation may be found in the header file examples.h
 */

#include <stdlib.h>
#include <string.h>
#include "inc/examples.h"

int ex_cycle(dfa_t *dfa, int size) {
    int output = -1;
    if (size == 0 || dfa == NULL) {
        return -1;
    }
    bool *main_finals = NULL;
    int *transitions = NULL;

    main_finals = calloc(size, sizeof(bool));
    transitions = malloc(size * 2 * sizeof(int));
    if (main_finals == NULL || transitions == NULL) {
        goto failure;
    }
    int symbols[2] = {44, 55};
    for (int i = 0; i < size; i++) {
        transitions[i * 2] = (i + 1) % size;
        transitions[i * 2 + 1] = (i - 1) % size;
    }
    output = DFA_new(dfa, size, 2, 0, main_finals, symbols, transitions);

    failure:
        free(transitions);
        free(main_finals);
        return output;
}


int ex_cdm(dfa_t *dfa) {
    if (dfa == NULL) {
        return -1;
    }
    bool finals[6];
    memset(finals, 0, 6 * sizeof(bool));
    int transitions[6][2] = { {1,1}, {2,3}, {4,5}, {5,4}, {5,5}, {-1,-1} };
    int *trans = malloc(12 * sizeof(int));
    if (trans == NULL) {
        return -1;
    }

    memcpy(trans, transitions, 12 * sizeof(int));
    int symbols[2] = {44, 55};
    int out = DFA_new(dfa, 6, 2, 0, finals, symbols, trans);

    free(trans);
    return out;
}

/* Patterns */

int ex_linear(dfa_t *dfa) {
    if (dfa == NULL) {
        return -1;
    }
    const int NUM_STATES = 3;
    bool finals[NUM_STATES];
    memset(finals, 0, NUM_STATES * sizeof(bool));
    int transitions[NUM_STATES][2] = { {1,1}, {2,2}, {-1,-1} };
    int *trans = malloc(2 * NUM_STATES * sizeof(int));
    if (trans == NULL) {
        return -1;
    }

    memcpy(trans, transitions, 2 * NUM_STATES * sizeof(int));
    int symbols[2] = {67, 68};
    int out = DFA_new(dfa, NUM_STATES, 2, 0, finals, symbols, trans);

    free(trans);
    return out;
}
int ex_twoway(dfa_t *dfa) {
    if (dfa == NULL) {
        return -1;
    }
    const int NUM_STATES = 3;
    bool finals[NUM_STATES];
    memset(finals, 0, NUM_STATES * sizeof(bool));
    int transitions[NUM_STATES][2] = { {1,-1}, {2,0}, {-1,1} };
    int *trans = malloc(2 * NUM_STATES * sizeof(int));
    if (trans == NULL) {
        return -1;
    }

    memcpy(trans, transitions, 2 * NUM_STATES * sizeof(int));
    int symbols[2] = {67, 68};
    int out = DFA_new(dfa, NUM_STATES, 2, 0, finals, symbols, trans);

    free(trans);
    return out;
}
int ex_sink(dfa_t *dfa) {
    if (dfa == NULL) {
        return -1;
    }
    const int NUM_STATES = 3;
    bool finals[NUM_STATES];
    memset(finals, 0, NUM_STATES * sizeof(bool));
    int transitions[NUM_STATES][2] = { {1,2}, {2,2}, {-1,-1} };
    int *trans = malloc(2 * NUM_STATES * sizeof(int));
    if (trans == NULL) {
        return -1;
    }

    memcpy(trans, transitions, 2 * NUM_STATES * sizeof(int));
    int symbols[2] = {67, 68};
    int out = DFA_new(dfa, NUM_STATES, 2, 0, finals, symbols, trans);

    free(trans);
    return out;
}

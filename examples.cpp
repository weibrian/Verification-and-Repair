/** @file examples.c
 *  @brief Example state machines and patterns
 *  @author Brian Wei
 *
 *  Detailed documentation may be found in the header file examples.h
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include "inc/examples.h"
#include "inc/ltsa_parser.h"

int ex_cycle(dfa_t *dfa, int size) {
    int output = -1;
    if (size == 0 || dfa == nullptr) {
        return -1;
    }
    bool *main_finals = nullptr;
    int *transitions = nullptr;
//    int symbols[2] = {44, 55};
    auto *symbols = new std::vector<std::string>({"A", "B"});

    main_finals = (bool*)calloc(size, sizeof(bool));
    transitions = (int*)malloc(size * 2 * sizeof(int));
    if (main_finals == nullptr || transitions == nullptr) {
        goto failure;
    }
    for (int i = 0; i < size; i++) {
        transitions[i * 2] = (i + 1) % size;
        transitions[i * 2 + 1] = (i - 1) % size;
    }
    output = DFA_new(dfa, size, 2, 0, main_finals, *symbols, transitions);

    failure:
        free(transitions);
        free(main_finals);
        return output;
}


int ex_cdm(dfa_t *dfa) {
    if (dfa == nullptr) {
        return -1;
    }
    bool finals[6];
    memset(finals, 0, 6 * sizeof(bool));
    int transitions[6][2] = { {1,1}, {2,3}, {4,5}, {5,4}, {5,5}, {DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL} };
    int *trans = (int*)malloc(12 * sizeof(int));
    if (trans == nullptr) {
        return -1;
    }

    memcpy(trans, transitions, 12 * sizeof(int));
    auto *symbols = new std::vector<std::string>({"A", "B"});

    int out = DFA_new(dfa, 6, 2, 0, finals, *symbols, trans);

    free(trans);
    return out;
}

/* Patterns */

int ex_linear(dfa_t *dfa) {
    if (dfa == nullptr) {
        return -1;
    }
    const int NUM_STATES = 3;
    bool finals[NUM_STATES];
    memset(finals, 0, NUM_STATES * sizeof(bool));
    int transitions[NUM_STATES][2] = { {1,1}, {2,2}, {DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL} };
    int *trans = (int*)malloc(2 * NUM_STATES * sizeof(int));
    if (trans == nullptr) {
        return -1;
    }

    memcpy(trans, transitions, 2 * NUM_STATES * sizeof(int));
//    int symbols[2] = {67, 68};
    auto *symbols = new std::vector<std::string>({"A", "B"});

    int out = DFA_new(dfa, NUM_STATES, 2, 0, finals, *symbols, trans);

    free(trans);
    return out;
}
int ex_twoway(dfa_t *dfa) {
    if (dfa == nullptr) {
        return -1;
    }
    const int NUM_STATES = 3;
    bool finals[NUM_STATES];
    memset(finals, 0, NUM_STATES * sizeof(bool));
    int transitions[NUM_STATES][2] = { {1,DFA_DUMMY_SYMBOL}, {2,0}, {DFA_DUMMY_SYMBOL,1} };
    int *trans = (int*)malloc(2 * NUM_STATES * sizeof(int));
    if (trans == nullptr) {
        return -1;
    }

    memcpy(trans, transitions, 2 * NUM_STATES * sizeof(int));
//    int symbols[2] = {67, 68};
    auto *symbols = new std::vector<std::string>({"A", "B"});

    int out = DFA_new(dfa, NUM_STATES, 2, 0, finals, *symbols, trans);

    free(trans);
    return out;
}
int ex_sink(dfa_t *dfa) {
    if (dfa == nullptr) {
        return -1;
    }
    const int NUM_STATES = 3;
    bool finals[NUM_STATES];
    memset(finals, 0, NUM_STATES * sizeof(bool));
    int transitions[NUM_STATES][2] = { {1,2}, {2,2}, {DFA_DUMMY_SYMBOL, DFA_DUMMY_SYMBOL} };
    int *trans = (int*)malloc(2 * NUM_STATES * sizeof(int));
    if (trans == nullptr) {
        return -1;
    }

    memcpy(trans, transitions, 2 * NUM_STATES * sizeof(int));
//    int symbols[2] = {67, 68};
    auto *symbols = new std::vector<std::string>({"A", "B"});

    int out = DFA_new(dfa, NUM_STATES, 2, 0, finals, *symbols, trans);

    free(trans);
    return out;
}


int ex_fancy(dfa_t *dfa) {
    if (dfa == nullptr) {
        return -1;
    }
    const int NUM_STATES = 3;
    bool finals[NUM_STATES];
    memset(finals, 0, NUM_STATES * sizeof(bool));
    int transitions[NUM_STATES][2] = { {1,2}, {2,2}, {0,0} };
    int *trans = (int*)malloc(2 * NUM_STATES * sizeof(int));
    if (trans == nullptr) {
        return -1;
    }

    memcpy(trans, transitions, 2 * NUM_STATES * sizeof(int));
//    int symbols[2] = {67, 68};
    auto *symbols = new std::vector<std::string>({"A", "B"});

    int out = DFA_new(dfa, NUM_STATES, 2, 0, finals, *symbols, trans);

    free(trans);
    return out;
}


int prop_five(dfa_t *dfa) {
    if (dfa == nullptr) {
        return -1;
    }
    const int NUM_STATES = 6;
    bool finals[NUM_STATES];
    memset(finals, 0, NUM_STATES * sizeof(bool));
    int transitions[NUM_STATES][1] = { {1}, {2}, {3}, {4}, {5}, {DFA_DUMMY_SYMBOL} };
    int *trans = (int*)malloc(NUM_STATES * sizeof(int));
    if (trans == nullptr) {
        return -1;
    }

    memcpy(trans, transitions, NUM_STATES * sizeof(int));
//    int symbols[1] = {44};
    auto *symbols = new std::vector<std::string>({"A"});

    int out = DFA_new(dfa, NUM_STATES, 1, 0, finals, *symbols, trans);

    free(trans);
    return out;
}

int ex_infusion(dfa_t *dfa) {
    return parser_go(dfa, "/Users/bwei/CLionProjects/Verif/ltsa_models/infusion_pump_relaxed");
}

int ex_human(dfa_t *dfa) {
    return parser_go(dfa, "/Users/bwei/CLionProjects/Verif/ltsa_models/infusion_pump_human");
}

int ex_prop(dfa_t *dfa) {
    return parser_go(dfa, "/Users/bwei/CLionProjects/Verif/ltsa_models/infusion_pump_amtchecker");
}
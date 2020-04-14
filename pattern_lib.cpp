//
// Created by Brian Wei on 4/1/20.
//

#include "inc/pattern_lib.h"
#include "inc/DFA.h"
#include "inc/modify.h"

int patt_3state_template(dfa_t *dfa, int transitions[3][2]) {
    if (dfa == nullptr) {
        return -1;
    }
    const int NUM_STATES = 3;
    bool finals[NUM_STATES];
    memset(finals, 0, NUM_STATES * sizeof(bool));
    int *trans = (int*)malloc(2 * NUM_STATES * sizeof(int));
    if (trans == nullptr) {
        return -1;
    }

    memcpy(trans, transitions, 2 * NUM_STATES * sizeof(int));
    auto *symbols = new std::vector<std::string>({"A", "B"});

    int out = DFA_new(dfa, NUM_STATES, 2, 0, finals, *symbols, trans);

    free(trans);
    return out;
}

int patt_generic_pre(dfa_t *dfa) {
    int transitions[3][2] = { {1,DFA_DUMMY_SYMBOL}, {DFA_DUMMY_SYMBOL,2}, {DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL} };
    return patt_3state_template(dfa, transitions);
}

int patt_prematrurestart_post(dfa_t *dfa) {
    int transitions[3][2] = { {1,1}, {DFA_DUMMY_SYMBOL, 2}, {DFA_DUMMY_SYMBOL ,DFA_DUMMY_SYMBOL}};
    return patt_3state_template(dfa, transitions);
}

int patt_delaystart_post(dfa_t *dfa) {
    int transitions[3][2] = { {1, DFA_DUMMY_SYMBOL}, {2, 2}, {DFA_DUMMY_SYMBOL, DFA_DUMMY_SYMBOL}};
    return patt_3state_template(dfa, transitions);
}

int patt_omission_post(dfa_t *dfa) {
    int transitions[3][2] = { {2, 2}, {DFA_DUMMY_SYMBOL, 2}, {DFA_DUMMY_SYMBOL, DFA_DUMMY_SYMBOL}};
    return patt_3state_template(dfa, transitions);
}

int patt_reversal_post(dfa_t *dfa) {
    int transitions[3][2] = { {1, 1}, {2, 2}, {DFA_DUMMY_SYMBOL, DFA_DUMMY_SYMBOL}};
    return patt_3state_template(dfa, transitions);
}

int patt_intrusion_pre(dfa_t *dfa) {
    int transitions[3][2] = { {1, DFA_DUMMY_SYMBOL}, {DFA_DUMMY_SYMBOL, DFA_DUMMY_SYMBOL}, {DFA_DUMMY_SYMBOL, DFA_DUMMY_SYMBOL}};
    return patt_3state_template(dfa, transitions);
}

int patt_intrusion_post(dfa_t *dfa) {
    int transitions[3][2] = { {1, 1}, {2, DFA_DUMMY_SYMBOL}, {DFA_DUMMY_SYMBOL, DFA_DUMMY_SYMBOL}};
    return patt_3state_template(dfa, transitions);
}

int patt_repetition_post(dfa_t *dfa) {
    int transitions[3][2] = { {0, 1}, {0, 2}, {DFA_DUMMY_SYMBOL, DFA_DUMMY_SYMBOL}};
    return patt_3state_template(dfa, transitions);
}

int patternlib_init(mapping_list &mappings) {
    auto *prematurestart_pre = (dfa_t*)malloc(sizeof(dfa_t));
    auto *prematurestart_post = (dfa_t*)malloc(sizeof(dfa_t));
    auto *delaystart_pre = (dfa_t*)malloc(sizeof(dfa_t));
    auto *delaystart_post = (dfa_t*)malloc(sizeof(dfa_t));
    auto *omission_pre = (dfa_t*)malloc(sizeof(dfa_t));
    auto *omission_post = (dfa_t*)malloc(sizeof(dfa_t));
    auto *reversal_pre = (dfa_t*)malloc(sizeof(dfa_t));
    auto *reversal_post = (dfa_t*)malloc(sizeof(dfa_t));
    auto *intrusion_pre = (dfa_t*)malloc(sizeof(dfa_t));
    auto *intrusion_post = (dfa_t*)malloc(sizeof(dfa_t));
    auto *repetition_pre = (dfa_t*)malloc(sizeof(dfa_t));
    auto *repetition_post = (dfa_t*)malloc(sizeof(dfa_t));

    // TODO error check all of these
    patt_generic_pre(prematurestart_pre);
    patt_prematrurestart_post(prematurestart_post);
    modify_add_to_mappings(mappings, modify_new_pattern_map(prematurestart_pre, prematurestart_post, "prematurestart"));

    patt_generic_pre(delaystart_pre);
    patt_delaystart_post(delaystart_post);
    modify_add_to_mappings(mappings, modify_new_pattern_map(delaystart_pre, delaystart_post, "delaystart"));

    patt_generic_pre(omission_pre);
    patt_omission_post(omission_post);
    modify_add_to_mappings(mappings, modify_new_pattern_map(omission_pre, omission_post, "omission"));

    patt_generic_pre(reversal_pre);
    patt_reversal_post(reversal_post);
    modify_add_to_mappings(mappings, modify_new_pattern_map(reversal_pre, reversal_post, "reversal"));

    patt_intrusion_pre(intrusion_pre);
    patt_intrusion_post(intrusion_post);
    modify_add_to_mappings(mappings, modify_new_pattern_map(intrusion_pre, intrusion_post, "intrusion"));

    patt_generic_pre(repetition_pre);
    patt_repetition_post(repetition_post);
    modify_add_to_mappings(mappings, modify_new_pattern_map(repetition_pre, repetition_post, "repetition"));

    return 0;
}



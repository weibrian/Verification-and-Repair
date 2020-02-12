//
// Created by Brian Wei on 2/11/20.
//
#include "inc/Property.h"
#include "inc/array_util.h"
#include <iostream>
#include <unistd.h>
#include <queue>
#include <boost/unordered_set.hpp>


Property::Property(dfa_t *dfa, interps_t mode,
        int *error_states, int num_error_states) {
    this->sim_dfa = dfa;
    this->invalid_interp = mode;
    this->error_states = error_states;
    this->num_error_states = num_error_states;
}

void Property::property_print() {
    std::cout << "Simulating DFA: " << std::endl;
    DFA_print(this->sim_dfa, stdout);
    std::cout << "Interpretation mode: " << (this->invalid_interp == interps::NOP ? "NOP" : "ERROR") << std::endl;
    array_print(this->error_states, 1, num_error_states, stdout);
}
extern "C" void property_print_C(Property *p) {
    p->property_print();
}

typedef struct {
    int dfa_state;
    int prop_state;
} check_state;
typedef boost::unordered_set<check_state> unordered_set;
bool operator==(const check_state &lhs, const check_state &rhs) {
    return lhs.dfa_state == rhs.dfa_state && lhs.prop_state == rhs.prop_state;
}
std::size_t hash_value(const check_state &a) {
    std::size_t seed = 0;
    boost::hash_combine(seed, a.dfa_state);
    boost::hash_combine(seed, a.prop_state);
    return seed;
}

std::function<void(int)> real_sigalarm_handler;
void sig_alarm_handler(int sig) {
    real_sigalarm_handler(sig);
}
bool Property::property_check(dfa_t *dfa) {
    // Attn: this code is not thread safe
    int *valid_symbols = dfa->alphabet_symbols;
    int alphabet_size = dfa->alphabet_size;
    dfa_t *prop_dfa = this->sim_dfa;

    int alarm_status;

    real_sigalarm_handler = [&](int sig) {
        alarm_status = 0;
    };

    signal(SIGALRM, sig_alarm_handler);
    alarm(5);
    alarm_status = 1;


    std::queue<check_state*> todo_list;
    auto *first = new check_state;
    first->dfa_state = dfa->initial_state;
    first->prop_state = prop_dfa->initial_state;
    todo_list.push(first);

    unordered_set visited_states;
    visited_states.insert({dfa->initial_state, prop_dfa->initial_state});

    bool result = true;

    while(alarm_status) {
        if (todo_list.empty()) {
//            std::cout << "exit from empty queue" << std::endl;
            goto cleanup;
        }
        check_state *current = todo_list.front();
        todo_list.pop();
//        std::cout << "From (p = " << current->prop_state << ", m = " << current->dfa_state << ")" << std::endl;

        for (int symb_ind = 0; symb_ind < alphabet_size; symb_ind++) {
            auto *ck = new check_state;
            ck->dfa_state = DFA_apply_symbol(dfa, current->dfa_state, valid_symbols[symb_ind]);
            ck->prop_state = DFA_apply_symbol(prop_dfa, current->prop_state, valid_symbols[symb_ind]);
            if (ck->prop_state < 0) {
                ck->prop_state = current->prop_state;
            }
//            std::cout << "on " << valid_symbols[symb_ind] << " to (p = " << ck->prop_state << ", m = " << ck->dfa_state << ")" << std::endl;
            if (ck->dfa_state == DFA_DUMMY_SYMBOL) {
                continue;
            } else if (array_contains(this->error_states, ck->prop_state, this->num_error_states)) {
                result = false;
                goto cleanup;
            } else {
                if (visited_states.find({ck->dfa_state, ck->prop_state}) == visited_states.end()) {
                    todo_list.push(ck);
                }
            }
        }
    }

    cleanup:
    alarm(0);
    signal(SIGALRM, SIG_DFL);
    return result;
}
extern "C" bool property_check_C(Property *p, dfa_t *dfa) {
    return p->property_check(dfa);
}

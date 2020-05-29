/** @file Property.cpp
 *  @brief Implementation of properties including how to check them
 *  @author Brian Wei
 *
 *  Detailed Documentation in header file
 */
#include "inc/Property.h"
#include <iostream>
#include <unistd.h>
#include <queue>
#include <boost/unordered_set.hpp>


Property::Property(dfa& dfa, interps_t mode,
        int *error_states, int num_error_states) {
    this->sim_dfa = &dfa;
    this->invalid_interp = mode;
    this->error_states.insert(error_states, error_states + num_error_states);
}

void Property::property_print() {
    std::cout << "Simulating DFA: \n";
    this->sim_dfa->DFA_print(stdout);
    std::cout << "Interpretation mode: " << (this->invalid_interp == interps::NOP ? "NOP" : "ERROR") << std::endl;
    for(int i : error_states) std::cout << i << " ";
    std::cout << std::endl;
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
//    std::cout << "Property check terminating via timeout" << std::endl;
//    std::fflush(stdout);
    real_sigalarm_handler(sig);
}
bool Property::property_check(dfa &M) {
    // Attn: this code is not thread safe
    std::vector<std::string> valid_symbols = M.alphabet_symbols;
    int alphabet_size = M.alphabet_symbols.size();
    dfa *prop_dfa = this->sim_dfa;

    int alarm_status;

    real_sigalarm_handler = [&](int sig) {
        alarm_status = 0;
    };

    signal(SIGALRM, sig_alarm_handler);
    alarm(1);
    alarm_status = 1;


    std::queue<check_state*> todo_list;
    auto *first = new check_state;
    first->dfa_state = M.initial_state;
    first->prop_state = prop_dfa->initial_state;
    todo_list.push(first);

    unordered_set visited_states;
    visited_states.insert({M.initial_state, prop_dfa->initial_state});

    bool result = true;

    while(alarm_status) {
        if (todo_list.empty()) {
            goto cleanup;
        }
        check_state *current = todo_list.front();
        todo_list.pop();

        for (int symb_ind = 0; symb_ind < alphabet_size; symb_ind++) {
            auto *ck = new check_state;
            ck->dfa_state = M.DFA_apply_symbol(current->dfa_state, valid_symbols[symb_ind]);
            ck->prop_state = prop_dfa->DFA_apply_symbol(current->prop_state, valid_symbols[symb_ind]);
            if (ck->prop_state < 0) {
                ck->prop_state = current->prop_state;
            }
            if (ck->dfa_state == DFA_DUMMY_SYMBOL) {
                continue;
            } else if (this->error_states.find(ck->prop_state) != this->error_states.end()) {
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

//
// Created by Brian Wei on 2/11/20.
//
#include "inc/Property.h"
#include "inc/array_util.h"
#include <iostream>

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

bool property_check(dfa_t *dfa) {
    (void)dfa;
    return true;
}
extern "C" bool property_check_C(Property *p, dfa_t *dfa) {
    return p->property_check(dfa);
}

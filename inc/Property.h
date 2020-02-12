//
// Created by Brian Wei on 2/11/20.
//

#ifndef __VERIF_PROPERTY_H__
#define __VERIF_PROPERTY_H__

#include "DFA.h"

typedef enum class interps { NOP, ERROR } interps_t;

class Property {
private:
    dfa_t *sim_dfa;
    interps_t invalid_interp;
    int *error_states;
    int num_error_states;
public:
    Property(dfa_t *dfa, interps_t mode, int *error_states, int num_error_states);
    void property_print();
    bool property_check(dfa_t *dfa);
};

extern "C" void property_print_C(Property *p);
extern "C" bool property_check_C(Property *p, dfa_t *dfa);

#endif /* __VERIF_PROPERTY_H__ */

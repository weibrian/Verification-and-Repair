#include <iostream>

#include "inc/DFA.h"
#include "inc/examples.h"
#include "inc/Property.h"
#include "inc/modify.h"
#include "inc/pattern_lib.h"

int main() {
    dfa *machine_dfa = ex_infusion();
    dfa *human_dfa = ex_human();
    dfa *prop = ex_prop();

    prop->DFA_print(stdout);

    int err_states[1] = {8};
    Property p(*prop, interps::NOP, err_states, 1);

    std::cout << "Initial DFA -------------------------" << std::endl;
    machine_dfa->DFA_print(stdout);
    std::flush(std::cout);
    std::cout << "-------------------------------------" << std::endl;
    human_dfa->DFA_print(stdout);
    std::cout << "-------------------------------------" << std::endl;
    std::flush(std::cout);

    mapping_list mappings = modify_new_mapping();
    patternlib_init(mappings);
    std::cout << "Machine DFA has " << machine_dfa->num_states << " states" << std::endl;

    int res = modify_violate_property(*human_dfa, *machine_dfa, &p, &mappings, 9999);
    if (res == MODIFY_SUCCESSFUL) {
        std::cout << ">> Modify success -- now violates property" << std::endl;
        std::cout << "Modified DFA ------------------------" << std::endl;
        human_dfa->DFA_print(stdout);
        std::cout << "-------------------------------------" << std::endl;
    } else {
        std::cout << ">> Modify could not violate the property with given patterns" << std::endl;
    }

    return 0;
}


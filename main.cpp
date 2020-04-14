#include <iostream>

#include "inc/DFA.h"
#include "inc/array_util.h"
#include "inc/examples.h"
#include "inc/Property.h"
#include "inc/modify.h"
#include "inc/pattern_lib.h"

int main() {
    dfa_t main_dfa, machine_dfa, prop, human_dfa;
    if(ex_infusion(&machine_dfa)
       || ex_human(&human_dfa)
       || ex_prop(&prop)) {
        printf("DFA initialization error\n");
    }
    DFA_print(&prop, stdout);

    int err_states[1] = {8};
    Property p = Property(&prop, interps::NOP, err_states, 1);

    std::cout << "Initial DFA -------------------------" << std::endl;
    DFA_print(&machine_dfa, stdout);
    std::flush(std::cout);
    std::cout << "-------------------------------------" << std::endl;
    DFA_print(&human_dfa, stdout);
    std::cout << "-------------------------------------" << std::endl;
    std::flush(std::cout);

    mapping_list mappings = modify_new_mapping();
    patternlib_init(mappings);
    std::cout << "Machine DFA has " << machine_dfa.num_states << " states" << std::endl;

    int res = modify_violate_property(&human_dfa, &machine_dfa, &p, &mappings, 1);
    if (res == MODIFY_SUCCESSFUL) {
        std::cout << ">> Modify success -- now violates property" << std::endl;
        std::cout << "Modified DFA ------------------------" << std::endl;
        DFA_print(&human_dfa, stdout);
        std::cout << "-------------------------------------" << std::endl;
    } else {
        std::cout << ">> Modify could not violate the property with given patterns" << std::endl;
    }

    return 0;
}


#include <iostream>

#include "inc/DFA.h"
#include "inc/array_util.h"
#include "inc/examples.h"
#include "inc/Property.h"
#include "inc/modify.h"
#include "inc/pattern_lib.h"

int main() {
    dfa_t main_dfa, machine_dfa, prop, pattern_sink, pattern_linear, pattern_fancy, human_dfa;
    if(ex_infusion(&machine_dfa)
        || ex_human(&human_dfa)
        || ex_prop(&prop)
        || ex_sink(&pattern_sink)
        || ex_linear(&pattern_linear)
        || ex_fancy(&pattern_fancy)) {
        printf("DFA initialization error\n");
    }
//    ex_infusion(&machine_dfa);
    DFA_print(&prop, stdout);
//
//    ex_human(&human_dfa);
//
//    std::cout << "asdf" << std::endl;
//    DFA_print(&human_dfa, stdout);


    DFA_parallel(&main_dfa, &machine_dfa, &human_dfa);

      int err_states[1] = {17};
      Property p = Property(&prop, interps::NOP, err_states, 1);

    std::cout << "Initial DFA -------------------------" << std::endl;
    DFA_print(&machine_dfa, stdout);
    std::flush(std::cout);
    std::cout << "-------------------------------------" << std::endl;
    DFA_print(&human_dfa, stdout);
    std::cout << "-------------------------------------" << std::endl;
    DFA_print(&main_dfa, stdout);
    std::cout << "-------------------------------------" << std::endl;
    DFA_print(&prop, stdout);
    std::cout << "-------------------------------------" << std::endl;
    std::flush(std::cout);

//    return 0;
    bool result = p.property_check(&main_dfa);
    if (result) {
        std::cout << ">> Starts passing the property" << std::endl;
    } else {
        std::cout << ">> Starts failing the property" << std::endl;
    }
    std::flush(std::cout);

    mapping_list mappings = modify_new_mapping();
    patternlib_init(mappings);
//    modify_add_to_mappings(mappings, modify_new_pattern_map(&pattern_sink, &pattern_linear));
//    modify_add_to_mappings(mappings, modify_new_pattern_map(&pattern_sink, &pattern_fancy));

    int res = modify_violate_property(&main_dfa, &p, &mappings);
    if (res == MODIFY_SUCCESSFUL) {
        std::cout << ">> Modify success -- now violates property" << std::endl;
        std::cout << "Modified DFA ------------------------" << std::endl;
        DFA_print(&main_dfa, stdout);
        std::cout << "-------------------------------------" << std::endl;
    } else {
        std::cout << ">> Modify could not violate the property with given patterns" << std::endl;
    }

    return 0;
}


//#include <iostream>
//
//#include "inc/DFA.h"
//#include "inc/array_util.h"
//#include "inc/examples.h"
//#include "inc/Property.h"
//#include "inc/modify.h"
//
//int main() {
//    dfa_t main_dfa, prop, pattern_sink, pattern_linear, pattern_fancy;
//    if(ex_cdm(&main_dfa)
//       || prop_five(&prop)
//       || ex_sink(&pattern_sink)
//       || ex_linear(&pattern_linear)
//       || ex_fancy(&pattern_fancy)) {
//        printf("DFA initialization error\n");
//    }
//    int err_states[1] = {5};
//    Property p = Property(&prop, interps::NOP, err_states, 1);
//
//    std::cout << "Initial DFA -------------------------" << std::endl;
//    DFA_print(&main_dfa, stdout);
//    std::cout << "-------------------------------------" << std::endl;
//
//    bool result = p.property_check(&main_dfa);
//    if (result) {
//        std::cout << ">> Starts passing the property" << std::endl;
//    } else {
//        std::cout << ">> Starts failing the property" << std::endl;
//    }
//
//
//    mapping_list mappings = modify_new_mapping();
//    modify_add_to_mappings(mappings, modify_new_pattern_map(&pattern_sink, &pattern_linear));
//    modify_add_to_mappings(mappings, modify_new_pattern_map(&pattern_sink, &pattern_fancy));
//
//    int res = modify_violate_property(&main_dfa, &p, &mappings);
//    if (res == MODIFY_SUCCESSFUL) {
//        std::cout << ">> Modify success -- now violates property" << std::endl;
//        std::cout << "Modified DFA ------------------------" << std::endl;
//        DFA_print(&main_dfa, stdout);
//        std::cout << "-------------------------------------" << std::endl;
//    } else {
//        std::cout << ">> Modify could not violate the property with given patterns" << std::endl;
//    }
//
//    return 0;
//}
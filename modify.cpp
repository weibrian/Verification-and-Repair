//
// Created by Brian Wei on 2/12/20.
//

#include "inc/modify.h"
#include <cstdlib>
#include <iostream>

pattern_map_t *modify_new_pattern_map(dfa_t *pattern1, dfa_t *pattern2) {
    auto *new_map = (pattern_map_t*)malloc(sizeof(pattern_map_t));
    new_map->initial = pattern1;
    new_map->target = pattern2;
    return new_map;
}

mapping_list modify_new_mapping() {
    mapping_list m;
    return m;
}

void modify_add_to_mappings(mapping_list &current_map, pattern_map_t *next) {
    current_map.push_back(next);
}

int modify_violate_property(dfa_t *modification_dfa, dfa_t *machine_dfa, Property *p, mapping_list *maps, int max_per_map) {

//    assert(p->property_check(main_dfa));
    int succ_count = 0;
    dfa_t dest;
    auto *modification_dfa_copy = (dfa_t*)malloc(sizeof(dfa_t));
    if (modification_dfa_copy == nullptr)  {
        return MODIFY_INVALID_ARG;
    }
    int err_flag;
    for(auto const& map: *maps) {
        std::cout << "Map: ";
        for(int trial = 0; trial < max_per_map; trial++) {
//            std::cout << "trying a map on trial " << trial << std::endl;
            std::flush(std::cout);
            DFA_clone(modification_dfa, modification_dfa_copy);
            if ((err_flag = DFA_modify(modification_dfa_copy, map->initial, map->target, trial)) < 0) {
//                std::cout << "modification failed with code " << err_flag << std::endl;
//                std::flush(std::cout);
                std::cout << trial;
                break;
            } else {
                if ((err_flag = DFA_parallel(&dest, modification_dfa_copy, machine_dfa)) < 0) {
                    std::cout << "Composition Failed with code " << err_flag << std::endl;
                } else if (!(p->property_check(&dest))) {
//                    DFA_clone(modification_dfa_copy, modification_dfa);
//                    return MODIFY_SUCCESSFUL;
                    succ_count++;
                    std::cout << "!";
                } else {
                    std::cout << ".";
                }
            }
        }
        std::cout << std::endl;
    }
    free(modification_dfa_copy);
    std::cout << "Number of violating machines:" << succ_count << std::endl;
    return succ_count > 0 ? MODIFY_SUCCESSFUL : MODIFY_NOT_FOUND;
}


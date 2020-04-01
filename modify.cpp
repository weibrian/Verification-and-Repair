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

int modify_violate_property(dfa_t *main_dfa, Property *p, mapping_list *maps) {

//    assert(p->property_check(main_dfa));
    auto *main_dfa_copy = (dfa_t*)malloc(sizeof(dfa_t));
    if (main_dfa_copy == nullptr)  {
        return MODIFY_INVALID_ARG;
    }
    int err_flag;
    for(auto const& map: *maps) {
        std::cout << "trying a map" << std::endl;
        std::flush(std::cout);
        DFA_clone(main_dfa, main_dfa_copy);
        if ((err_flag = DFA_modify(main_dfa_copy, map->initial, map->target)) < 0) {
            std::cout << "modification failed with code " << err_flag << std::endl;
            std::flush(std::cout);
            continue;
        } else if (!(p->property_check(main_dfa_copy))) {
            DFA_clone(main_dfa_copy, main_dfa);
            return MODIFY_SUCCESSFUL;
        }
    }
    free(main_dfa_copy);
    return MODIFY_NOT_FOUND;
}


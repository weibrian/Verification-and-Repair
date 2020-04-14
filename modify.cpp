//
// Created by Brian Wei on 2/12/20.
//

#include "inc/modify.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>

pattern_map_t *modify_new_pattern_map(dfa_t *pattern1, dfa_t *pattern2, std::string name) {
    auto *new_map = (pattern_map_t*)malloc(sizeof(pattern_map_t));
    new_map->initial = pattern1;
    new_map->target = pattern2;
    new_map->name = new std::string(std::move(name));
    return new_map;
}

mapping_list modify_new_mapping() {
    mapping_list m;
    return m;
}

void modify_add_to_mappings(mapping_list &current_map, pattern_map_t *next) {
    current_map.push_back(next);
}

static int modify_single(mapping_list *maps, dfa_t *modification_dfa, int skip_count) {
    int err_flag, map_index = 0;
    pattern_map *curr_map = maps->at(map_index);
    int num = 0;
    while(skip_count --> 0) {
        if ((err_flag = DFA_modify(modification_dfa, curr_map->initial, curr_map->target, num++)) < 0) {
            std::cout << map_index << ", " << maps->size()  << ", " << err_flag << std::endl;
            if (map_index == maps->size() - 1) return MODIFY_NOT_FOUND;
           curr_map = maps->at(++map_index);
           num = 0;
           skip_count++;
        }
        std::cout << skip_count;
    }
    std::cout << "Returning a " << map_index << ": " << *(maps->at(map_index)->name) << std::endl;
    return MODIFY_SUCCESSFUL;
}

int modify_violate_property(dfa_t *modification_dfa, dfa_t *machine_dfa, Property *p,
        mapping_list *maps, int mod_depth) {

//    assert(p->property_check(main_dfa));
    int succ_count = 0, fail_count = 0;
    dfa_t dest;
    auto *modification_dfa_copy = (dfa_t*)malloc(sizeof(dfa_t) * mod_depth);
    if (modification_dfa_copy == nullptr)  {
        return MODIFY_MEMORY_ERR;
    }
    int err_flag;
    int mod_level = 0;
    int *skips = (int*)calloc(sizeof(int), mod_depth);
    while(1) {
        if (mod_level == 0) {
            DFA_clone(modification_dfa, &(modification_dfa_copy[0]));
        } else {
            DFA_clone(&(modification_dfa_copy[mod_level-1]), &(modification_dfa_copy[mod_level]));
        }
//        std::cout << mod_level << ":" << skips[mod_level] << std::endl;
        err_flag = modify_single(maps, &(modification_dfa_copy[mod_level]), skips[mod_level]++);
//        std::cout << "Level is " << mod_level << std::endl;
        if (mod_level == 0 && err_flag < 0) break;
        else if (err_flag < 0) {
            std::cout << "\n Level going down to " << mod_level-1 << std::endl;
            skips[mod_level] = 0;
            mod_level--;
            continue;
        } else if (mod_level == mod_depth - 1) {
            err_flag = DFA_parallel(&dest, &(modification_dfa_copy[mod_level]), machine_dfa);
            if (err_flag < 0) {
                std::cout << "Composition Failed with code " << err_flag << std::endl;
                break;
            } else if (!(p->property_check(&dest))) {
//                DFA_print(modification_dfa_copy, stdout);
                succ_count++;
                std::cout << "!";
            } else {
                fail_count++;
                std::cout << ".";
            }
        } else {
            mod_level++;
        }
        std::flush(std::cout);
    }
    std::cout << "\nGenerated " << succ_count + fail_count << " machines; " << succ_count << " violate" << std::endl;


//    for(auto const& map: *maps) {
//        std::cout << "Map: ";
//        for(int trial = 0; trial < max_per_map; trial++) {
////            std::cout << "trying a map on trial " << trial << std::endl;
//            std::flush(std::cout);
//            DFA_clone(modification_dfa, modification_dfa_copy);
//
//            if ((err_flag = DFA_modify(modification_dfa_copy, map->initial, map->target, trial)) < 0) {
////                std::cout << "modification failed with code " << err_flag << std::endl;
////                std::flush(std::cout);
//                std::cout << trial;
//                break;
//            } else {
//                if ((err_flag = DFA_parallel(&dest, modification_dfa_copy, machine_dfa)) < 0) {
//                    std::cout << "Composition Failed with code " << err_flag << std::endl;
//                } else if (!(p->property_check(&dest))) {
////                    DFA_clone(modification_dfa_copy, modification_dfa);
////                    return MODIFY_SUCCESSFUL;
//                    DFA_print(modification_dfa_copy, stdout);
//                    succ_count++;
//                    std::cout << "!";
//                } else {
//                    std::cout << ".";
//                }
//            }
//        }
//        std::cout << std::endl;
//    }
    free(modification_dfa_copy);
    free(skips);
//    std::cout << "Number of violating machines:" << succ_count << std::endl;
    return succ_count > 0 ? MODIFY_SUCCESSFUL : MODIFY_NOT_FOUND;
}


/** @file modify.c
 *  @brief Modification of DFAs via patterns
 *  @author Brian Wei
 *
 *  Detailed documentation in header file
 */

#include "inc/modify.h"
#include <cstdlib>
#include <iostream>

pattern_map_t *modify_new_pattern_map(dfa &pattern1, dfa &pattern2) {
    auto *new_map = new pattern_map_t;
    new_map->initial = &pattern1;
    new_map->target = &pattern2;
    return new_map;
}

mapping_list modify_new_mapping() {
    mapping_list m;
    return m;
}

void modify_add_to_mappings(mapping_list &current_map, pattern_map_t &next) {
    current_map.push_back(&next);
}

int modify_violate_property(dfa &modification_dfa, dfa &machine_dfa, Property *p,
        mapping_list *maps, int max_per_map, int modification_depth) {
    int succ_count = 0;
    dfa *dest;
    int err_flag;

    std::vector<dfa*> modification_dfa_copies (modification_depth, NULL);
    std::vector<int> trials (modification_depth, 0);
    std::vector<int> map_inds (modification_depth, 0);

    int current_depth = 0;
    bool keep_going = true;
    while(keep_going) {
        bool breaking_maploop = false;
        //std::cout << current_depth;
        if (current_depth == modification_depth) {
            dest = new dfa(*modification_dfa_copies[modification_depth-1], machine_dfa);
            if (!(p->property_check(*dest))) {
                // char outname[8];
                // snprintf(outname, 8, "%d.out", succ_count);
                // FILE *outfile = fopen(outname, "w+");
                // if (outfile == nullptr) {
                //     perror("Error saving output: ");
                //     exit(1);
                // }
                // modification_dfa.DFA_print(outfile);
                // fclose(outfile);
                succ_count++;
                std::cout << "!";
            } else {
                // std::cout << "." << trials[0] << " " << map_inds[0] << "\n";
            }
            current_depth = modification_depth - 1;
        } else {
            for (int map_ind = map_inds[current_depth]; map_ind < (*maps).size(); map_ind++) {
                auto map = (*maps)[map_ind];
                int trial = 0;
                if (map_ind == map_inds[current_depth]) {
                    trial = trials[current_depth] + 1;
                }
                for (; trial < max_per_map; trial++) {
                    if (current_depth == 0)
                        std::cout << map_ind << ", " << trial << "\n";

                    // std::cout << map_ind << " " << trial << "\n";
                    if (current_depth == 0) {
                        modification_dfa_copies[current_depth] = new dfa(modification_dfa);
                    } else {
                        modification_dfa_copies[current_depth] = new dfa(*modification_dfa_copies[current_depth-1]);
                    }
                    if ((err_flag = modification_dfa_copies[current_depth]->DFA_modify(*(map->initial), *(map->target), trial)) < 0) {
                        // failed to find one
                        trial = max_per_map;
                        goto after_trialloop;
                    } else {
                        // found one
                        trials[current_depth] = trial;
                        map_inds[current_depth] = map_ind;
                        // std::cout << "Found one where trial = " << trial << " and map_ind = " << map_ind << "\n";
                        current_depth++;
                        breaking_maploop = true;
                        goto after_maploop;
                    }
                }
                after_trialloop:;
            }
            after_maploop:;
            if (!breaking_maploop) {
                if (current_depth > 0) {
                    trials[current_depth] = 0;
                    map_inds[current_depth] = 0;
                    current_depth--;
                }
                else keep_going = false;
            }
        }

    }

    std::cout << "Number of violating machines:" << succ_count << std::endl;
    return succ_count > 0 ? MODIFY_SUCCESSFUL : MODIFY_NOT_FOUND;
}


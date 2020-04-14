//
// Created by Brian Wei on 2/12/20.
//

#ifndef __VERIF_MODIFY_H__
#define __VERIF_MODIFY_H__

#include "DFA.h"
#include "Property.h"
#include <vector>
#include <string>

#define MODIFY_SUCCESSFUL   (0)
#define MODIFY_INVALID_ARG  (-1)
#define MODIFY_MEMORY_ERR   (-2)
#define MODIFY_NOT_FOUND    (-3)

typedef struct pattern_map {
    dfa_t *initial;
    dfa_t *target;
    std::string *name;
} pattern_map_t;
typedef std::vector<pattern_map_t*> mapping_list;

pattern_map_t *modify_new_pattern_map(dfa_t *pattern1, dfa_t *pattern2, std::string name);
mapping_list modify_new_mapping();
void modify_add_to_mappings(mapping_list &current_map, pattern_map_t *next);
int modify_violate_property(dfa_t *modification_dfa, dfa_t *machine_dfa, Property *p, mapping_list *maps, int max_per_map);

#endif /* __VERIF_MODIFY_H__ */

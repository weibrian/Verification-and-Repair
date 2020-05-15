/** @file modify.h
 *  @brief Header for modification
 *  @author Brian Wei
 *
 *  Modify will make modifications to DFAs based on patterns. The
 *  modification algorithm will aim to find instances of patterns in the
 *  state machines and use the mappings list to make a replacement to
 *  violate a provided property
 */

#ifndef __VERIF_MODIFY_H__
#define __VERIF_MODIFY_H__

#include "DFA.h"
#include "Property.h"
#include <vector>

#define MODIFY_SUCCESSFUL   (0)
#define MODIFY_INVALID_ARG  (-1)
#define MODIFY_MEMORY_ERR   (-2)
#define MODIFY_NOT_FOUND    (-3)

/* Structure for pattern maps -- DFAs for the initial and target
 * configurations */
typedef struct pattern_map {
    dfa_t *initial;
    dfa_t *target;
} pattern_map_t;

/* type definition for mapping_list -- a list of pattern maps */
typedef std::vector<pattern_map_t*> mapping_list;

/** Create a new pattern map
 *
 * @param pattern1 First pattern, the initial pattern in the map
 * @param pattern2 Second pattter, the target pattern in the map
 * @return a pointer to a pattern_map; created via malloc
 */
pattern_map_t *modify_new_pattern_map(dfa_t *pattern1, dfa_t *pattern2);

/** @brief Creates a new mapping_list
 *
 * @return mapping_list with no initial mappings
 */
mapping_list modify_new_mapping();

/** @brief Add a pattern map to a matching list
 *
 * @param current_map Reference to mapping list to add to
 * @param next Pointer to pattern map to add
 */
void modify_add_to_mappings(mapping_list &current_map, pattern_map_t *next);

/** @brief Modify a DFA such that it will violate a property
 *
 * Make modifications to the modification DFA such that its parallel composition with the
 * machine DFA will lead to a violation of the property.  This is done by finding all occurrences
 * of the initial pattern DFAs in the modification DFA and replacing them with the targets,
 * evaluating whether there would be a property violation.  All violating state machines are saved
 * to output files.
 *
 * @param modification_dfa DFA that will be modified, typically the human model
 * @param machine_dfa DFA representing the machine
 * @param p Property that is aimed to be violated
 * @param maps List of pattern maps that can be used
 * @param max_per_map Limit on the number of attempted modifications per map
 * @return zero on success, negative error code on error or if no violating modifications
 *          are found
 */
int modify_violate_property(dfa_t *modification_dfa, dfa_t *machine_dfa, Property *p,
        mapping_list *maps, int max_per_map);

#endif /* __VERIF_MODIFY_H__ */

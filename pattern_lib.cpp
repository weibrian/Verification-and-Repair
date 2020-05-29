/** @file pattern_lib.cpp
 *  @brief Pattern Library
 *  @author Brian Wei
 *
 *  Detailed Documentation in header file
 */

#include "inc/DFA.h"
#include "inc/modify.h"

/** @brief A template for patterns with 3 states in start and end
 *
 * @param dfa A pointer to DFA which will contain te pattern
 * @param transitions Transition matrix for the corresponding pattern
 * @return zero on success, negative error code on failure
 */
static dfa *patt_3state_template(int transitions[3][2]);

/** @brief Initialize a dfa with a pattern with the generic starting configuration
 *
 * @param dfa A pointer to DFA which will contain the pattern
 * @return zero on success, negative error code on failure
 */
static dfa *patt_generic_pre();

/** @brief Initialize a dfa with pattern for target of premature start
 *
 * @param dfa A pointer to DFA which will contain the pattern
 * @return zero on success, negative error code on failure
 */
static dfa *patt_prematrurestart_post();

/** @brief Initialize a dfa with pattern for target of delayed start
 *
 * @param dfa A pointer to DFA which will contain the pattern
 * @return zero on success, negative error code on failure
 */
static dfa *patt_delaystart_post();

/** @brief Initialize a dfa with pattern for target of omission
 *
 * @param dfa A pointer to DFA which will contain the pattern
 * @return zero on success, negative error code on failure
 */
static dfa *patt_omission_post();

/** @brief Initialize a dfa with pattern for target of reversal
 *
 * @param dfa A pointer to DFA which will contain the pattern
 * @return zero on success, negative error code on failure
 */
static dfa *patt_reversal_post();

/** @brief Initialize a dfa with pattern for source of intrusion
 *
 * @param dfa A pointer to DFA which will contain the pattern
 * @return zero on success, negative error code on failure
 */
static dfa *patt_intrusion_pre();

/** @brief Initialize a dfa with pattern for target of intrusion
 *
 * @param dfa A pointer to DFA which will contain the pattern
 * @return zero on success, negative error code on failure
 */
static dfa *patt_intrusion_post();

/** @brief Initialize a dfa with pattern for target of repetition
 *
 * @param dfa A pointer to DFA which will contain the pattern
 * @return zero on success, negative error code on failure
 */
static dfa *patt_repetition_post();


/* *****     IMPLEMENTATION     ***** */

static dfa *patt_3state_template(int *transitions) {
    const int NUM_STATES = 3;
    auto finals = std::vector<bool>(NUM_STATES, false);

    auto symbols = std::vector<std::string>({"A", "B"});

    return new dfa(NUM_STATES, 2, 0, finals,
            symbols, transitions);
}

static dfa *patt_generic_pre() {
    int *transitions = new int[6] {1,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL,2,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL};
    return patt_3state_template(transitions);
}

static dfa *patt_prematrurestart_post() {
    int *transitions = new int[6] {1,1,DFA_DUMMY_SYMBOL,2,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL};
    return patt_3state_template(transitions);
}

static dfa *patt_delaystart_post() {
    int *transitions = new int[6] {1,DFA_DUMMY_SYMBOL,2,2,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL};
    return patt_3state_template(transitions);
}

static dfa *patt_omission_post() {
    int *transitions = new int[6] {2,2,DFA_DUMMY_SYMBOL,2,DFA_DUMMY_SYMBOL, DFA_DUMMY_SYMBOL};
    return patt_3state_template(transitions);
}

static dfa *patt_reversal_post() {
    int *transitions = new int[6] {1,1,2,2,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL};
    return patt_3state_template(transitions);
}

static dfa *patt_intrusion_pre() {
    int *transitions = new int[6] {1,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL};
    return patt_3state_template(transitions);
}

static dfa *patt_intrusion_post() {
    int *transitions = new int[6] {1,1,2,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL};
    return patt_3state_template(transitions);
}

static dfa *patt_repetition_post() {
    int *transitions = new int[6] {0,1,0,2,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL};
    return patt_3state_template(transitions);
}

void patternlib_init(mapping_list &mappings) {

    auto prematurestart_pre = patt_generic_pre();
    auto prematurestart_post = patt_prematrurestart_post();
    modify_add_to_mappings(mappings, *modify_new_pattern_map(*prematurestart_pre, *prematurestart_post));

    auto delaystart_pre = patt_generic_pre();
    auto delaystart_post = patt_delaystart_post();
    modify_add_to_mappings(mappings, *modify_new_pattern_map(*delaystart_pre, *delaystart_post));

    auto omission_pre = patt_generic_pre();
    auto omission_post = patt_omission_post();
    modify_add_to_mappings(mappings, *modify_new_pattern_map(*omission_pre, *omission_post));

    auto reversal_pre = patt_generic_pre();
    auto reversal_post = patt_reversal_post();
    modify_add_to_mappings(mappings, *modify_new_pattern_map(*reversal_pre, *reversal_post));

    auto intrusion_pre = patt_intrusion_pre();
    auto intrusion_post = patt_intrusion_post();
    modify_add_to_mappings(mappings, *modify_new_pattern_map(*intrusion_pre, *intrusion_post));

    auto repetition_pre = patt_generic_pre();
    auto repetition_post = patt_repetition_post();
    modify_add_to_mappings(mappings, *modify_new_pattern_map(*repetition_pre, *repetition_post));
}



#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "inc/DFA.h"
#include "inc/array_util.h"

static int get_next_permutation(int *current_permutation, int length, int max_value);

/** @brief Constructs a new DFA
 *
 * @param dfa
 * @param num_states
 * @param alphabet_size
 * @param finals
 * @param transition_matrix
 * @return zero on success, negative error code on failure
 */
int DFA_new(dfa_t *dfa, int num_states, int alphabet_size, int initial_state,
        const bool finals[num_states], int symbols[alphabet_size],
        int transition_matrix[num_states][alphabet_size]) {

    if (dfa == NULL) {
        return -1;
    }

    dfa->num_states = num_states;
    dfa->alphabet_size = alphabet_size;

    // initial state must be in the enumeration
    if (initial_state >= num_states || initial_state < 0) {
        return -1;
    }


    dfa->final_states = NULL;
    dfa->transition_matrix = NULL;
    dfa->alphabet_symbols = NULL;

    dfa->final_states = calloc(num_states, sizeof(bool));
    dfa->transition_matrix = calloc(num_states * alphabet_size, sizeof(int));
    dfa->alphabet_symbols = calloc(alphabet_size, sizeof(int));

    if (dfa->final_states == NULL || dfa->transition_matrix == NULL || dfa->alphabet_symbols == NULL) {
        goto failure;
    }


    memcpy(dfa->alphabet_symbols, symbols, alphabet_size * sizeof(int));

    for(int state = 0; state < num_states; state++) {
        dfa->final_states[state] = finals[state];
        for (int letter = 0; letter < alphabet_size; letter++) {
            int target_state = transition_matrix[state][letter];
            if (target_state >= num_states) {
                goto failure;
            }
            dfa->transition_matrix[state * alphabet_size + letter] = target_state;
        }
    }
    array_print(dfa->transition_matrix, 4, 2);
    return 0;

    failure:
        free(dfa->final_states);
        free(dfa->transition_matrix);
        free(dfa->alphabet_symbols);
        return -1;
}

int get_symbol_index(dfa_t *dfa, int symbol) {
    assert(dfa != NULL);
    for (int i = 0; i < dfa->alphabet_size; i++) {
        if (dfa->alphabet_symbols[i] == symbol) {
            return i;
        }
    }
    // not found
    return INVALID_SYMBOL;
}

/** @brief Runs a trace through a DFA
 *
 * @param dfa
 * @param trace
 * @param length
 * @return 1 if accept, 0 if reject, negative error code if other failure
 */
int DFA_run_trace(dfa_t *dfa, const int *trace, int length) {

    if (dfa == NULL || length < 0) {
        return -1;
    }

    int current_state = dfa->initial_state;
    if (current_state < 0 || current_state >= dfa->num_states) {
        return -1;
    }
    int num_states = dfa->num_states;
    for (int i = 0; i < length; i++) {
        int symbol_index = get_symbol_index(dfa, trace[i]);
        if (symbol_index < 0) {
            return -1;
        }
        current_state= dfa->transition_matrix[current_state * num_states + symbol_index];
        if (current_state < 0) {
            return 0;
        }
    }
    return dfa->final_states[current_state];
}

int *DFA_find_pattern(dfa_t *main_dfa, dfa_t *pattern) {
    if (main_dfa == NULL || pattern == NULL) {
        return NULL;
    }
    int pattern_states = pattern->num_states;
    int main_states = main_dfa->num_states;
    int main_alphabet_size = main_dfa->alphabet_size;
    if (pattern_states > main_states) {
        return NULL;
    }
    int *matching = malloc(pattern_states * sizeof(int));
    memset(matching, 0, pattern_states * sizeof(int));

    while(get_next_permutation(matching, pattern_states, main_states - 1) == 0) {
        array_print(matching, 1, pattern_states);

        int reduced_symbol_table[main_alphabet_size][pattern_states];

        for (int i = 0; i < main_alphabet_size; i++) {
            for(int j = 0; j < pattern_states; j++) {
                int index, dest = (main_dfa->transition_matrix)[matching[j] * main_alphabet_size + i];
                if ((index = array_index_of(matching, dest, pattern_states, 1)) >= 0) {
                    reduced_symbol_table[i][j] = index;
                } else {
                    reduced_symbol_table[i][j] = -1;
                }
            }
        }
        printf("%d; %d\n", main_alphabet_size, pattern_states);
        array_print((const int *)reduced_symbol_table, main_alphabet_size, pattern_states);
        break;
    }
    return NULL;
}

/** @brief Sets the current_permutation array to the next permutation
 *
 * Finds the lexicographically next permutation from the inputted current permutation,
 * where no value will exceed max_value.  Behaviour is undefined if the input
 * is invalid.
 *
 * @param current_permutation Current permutation
 * @param length Length of the array
 * @param max_value Maximum value an entry in the permutation can take
 * @return 0 on success, negative error code on failure
 */
static int get_next_permutation(int *current_permutation, int length, int max_value) {
    if (current_permutation == NULL) {
        return -1;
     }

    start_of_loop:
    for(int i = length - 1; i >= 0; i--) {
        int new_val = current_permutation[i]++;
        if (new_val == max_value) {
            current_permutation[i] = 0;
            continue;
        }
        if (array_contains_duplicates(current_permutation, length)) {
            goto start_of_loop;
        } else {
            return 0;
        }
    }
    return -1;
}


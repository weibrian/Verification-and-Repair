/** @file DFA.c
 *  @brief Source file for DFA structure
 *  @author Brian Wei
 *
 *  Detailed documentation may be found in the header file DFA.h
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "inc/DFA.h"
#include "inc/array_util.h"

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
static int get_next_permutation(int *current_permutation, int length, int max_value);

/* *****     IMPLEMENTATION     ***** */

int DFA_new(dfa_t *dfa, int num_states, int alphabet_size, int initial_state,
        const bool *finals, const int *symbols,
        const int *transition_matrix) {
    int error_code;
    if (dfa == NULL) {
        return DFA_INVALID_ARG;
    }

    dfa->num_states = num_states;
    dfa->alphabet_size = alphabet_size;

    // initial state must be in the enumeration
    if (initial_state >= num_states || initial_state < 0) {
        return DFA_INVALID_ARG;
    }

    dfa->final_states = NULL;
    dfa->transition_matrix = NULL;
    dfa->alphabet_symbols = NULL;

    dfa->final_states = calloc(num_states, sizeof(bool));
    dfa->transition_matrix = calloc(num_states * alphabet_size, sizeof(int));
    dfa->alphabet_symbols = calloc(alphabet_size, sizeof(int));

    if (dfa->final_states == NULL || dfa->transition_matrix == NULL || dfa->alphabet_symbols == NULL) {
        error_code = DFA_MEMORY_ERROR;
        goto failure;
    }


    memcpy(dfa->alphabet_symbols, symbols, alphabet_size * sizeof(int));

    for(int state = 0; state < num_states; state++) {
        dfa->final_states[state] = finals[state];
        for (int letter = 0; letter < alphabet_size; letter++) {
            int target_state = transition_matrix[state * alphabet_size + letter];
            if (target_state >= num_states) {
                error_code = DFA_INVALID_ARG;
                goto failure;
            }
            dfa->transition_matrix[state * alphabet_size + letter] = target_state;
        }
    }
    return 0;

    failure:
        free(dfa->final_states);
        free(dfa->transition_matrix);
        free(dfa->alphabet_symbols);
        return error_code;
}

int get_symbol_index(dfa_t *dfa, int symbol) {
    assert(dfa != NULL);
    int result = array_index_of(dfa->alphabet_symbols, symbol, dfa->alphabet_size, 1);
    return result < 0 ? DFA_INVALID_SYMBOL : result;
}

int DFA_run_trace(dfa_t *dfa, const int *trace, int length) {

    if (dfa == NULL || length < 0) {
        return DFA_INVALID_ARG;
    }

    int current_state = dfa->initial_state;
    if (current_state < 0 || current_state >= dfa->num_states) {
        return DFA_INVALID_ARG;
    }
    int num_states = dfa->num_states;
    for (int i = 0; i < length; i++) {
        int symbol_index = get_symbol_index(dfa, trace[i]);
        if (symbol_index < 0) {
            return DFA_INVALID_ARG;
        }
        current_state= dfa->transition_matrix[current_state * num_states + symbol_index];
        if (current_state < 0) {
            return 0;
        }
    }
    return dfa->final_states[current_state];
}

pattern_output_t *DFA_find_pattern(dfa_t *main_dfa, dfa_t *pattern) {
    int *symbol_permutation = NULL;
    int *matching = NULL;

    if (main_dfa == NULL || pattern == NULL) {
        goto failure;
    }
    int pattern_states = pattern->num_states;
    int main_states = main_dfa->num_states;
    int main_alphabet_size = main_dfa->alphabet_size;
    int pattern_alphabet_size = pattern->alphabet_size;

    if (pattern_states > main_states) {
        goto failure;
    }

    symbol_permutation = calloc(pattern_alphabet_size, sizeof(int));
    matching = calloc(pattern_states, sizeof(int));
    if (symbol_permutation == NULL || matching == NULL) {
        goto failure;
    }

    while(get_next_permutation(matching, pattern_states, main_states - 1) == 0) {

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

        while(get_next_permutation(symbol_permutation, pattern_alphabet_size, main_alphabet_size - 1) == 0) {
            for (int i = 0; i < pattern_alphabet_size; i++) {
                for (int j = 0; j < pattern_states; j++) {
                    int main_dest_val = reduced_symbol_table[symbol_permutation[i]][j];
                    int pattern_dest_val = pattern->transition_matrix[j * pattern_alphabet_size + i];
                    if (main_dest_val != pattern_dest_val) {
                        goto permutation_failed;
                    }
                }
            }
            for (int i = 0; i < pattern_alphabet_size; i++) {
                symbol_permutation[i] = main_dfa->alphabet_symbols[symbol_permutation[i]];
            }
            pattern_output_t *output = malloc(sizeof(pattern_output_t));
            if (output == NULL) {
                goto failure;
            }
            output->states = matching;
            output->symbols = symbol_permutation;
            return output;

            permutation_failed:
                continue;
        }
    }

    failure:
        free(symbol_permutation);
        free(matching);
        return NULL;
}

int DFA_modify(dfa_t *main_dfa, dfa_t *original_pattern, dfa_t *target_pattern) {
    if (main_dfa == NULL || original_pattern == NULL || target_pattern == NULL) {
        return DFA_INVALID_ARG;
    }
    if (original_pattern->num_states != target_pattern->num_states ||
            original_pattern->alphabet_size != target_pattern->alphabet_size) {
        return DFA_NOT_YET_IMPL;
    }
    pattern_output_t *pattern = DFA_find_pattern(main_dfa, original_pattern);
    if (pattern == NULL) {
        return DFA_PATTERN_NOT_FOUND;
    }
    int pattern_states = original_pattern->num_states;
    int pattern_alphabet = original_pattern->alphabet_size;
    int main_alphabet = main_dfa->alphabet_size;

    for(int state_no = 0; state_no < pattern_states; state_no++) {
        int state = pattern->states[state_no];
        for(int symbol_no = 0; symbol_no < pattern_alphabet; symbol_no++) {
            int symbol_ind = get_symbol_index(main_dfa, pattern->symbols[symbol_no]);
            if (symbol_ind == DFA_INVALID_SYMBOL) {
                return DFA_PATTERN_NOT_FOUND;
            }
            int target = target_pattern->transition_matrix[state_no * pattern_alphabet + symbol_no];
            main_dfa->transition_matrix[state * main_alphabet + symbol_ind] =
                    target == -1 ? -1 : pattern->states[target];
        }
    }

    DFA_free_pattern(pattern);
    return 0;
}

void DFA_free_pattern(pattern_output_t *pattern) {
    if (pattern == NULL) {
        return;
    }
    free(pattern->states);
    free(pattern->symbols);
    free(pattern);
}

void DFA_print(dfa_t *dfa, FILE *f) {
   fprintf(f, "Initial state: %d\n", dfa->initial_state);
   fprintf(f, "Final state(s): ");
   for(int i = 0; i < dfa->num_states; i++) {
       if (dfa->final_states[i]) fprintf(f, "%d ", i);
   }
   fprintf(f, "\nAlphabet Symbol(s): ");
   for(int i = 0; i < dfa->alphabet_size; i++) {
       fprintf(f, "%d ", dfa->alphabet_symbols[i]);
   }
   fprintf(f, "\nTransition Matrix:\n");
   array_print(dfa->transition_matrix, dfa->num_states, dfa->alphabet_size, f);
}

static int get_next_permutation(int *current_permutation, int length, int max_value) {
    assert(current_permutation != NULL);

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


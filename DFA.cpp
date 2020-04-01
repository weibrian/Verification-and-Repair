/** @file DFA.c
 *  @brief Source file for DFA structure
 *  @author Brian Wei
 *
 *  Detailed documentation may be found in the header file DFA.h
 */

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <set>
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
        const bool *finals, const std::vector<std::string>& symbols,
        const int *transition_matrix) {
    int error_code;
    if (dfa == nullptr) {
        return DFA_INVALID_ARG;
    }
    dfa->initial_state = initial_state;
    dfa->num_states = num_states;
    dfa->alphabet_size = alphabet_size;

    // initial state must be in the enumeration
    if (initial_state >= num_states || initial_state < 0) {
        return DFA_INVALID_ARG;
    }

    dfa->final_states = nullptr;
    dfa->transition_matrix = nullptr;

    dfa->final_states = (bool*)calloc(num_states, sizeof(bool));
    dfa->transition_matrix = (int*)calloc(num_states * alphabet_size, sizeof(int));
    dfa->alphabet_symbols = new std::vector<std::string>(symbols);
//            (std::string*)calloc(alphabet_size, sizeof(std::string));

    if (dfa->final_states == nullptr || dfa->transition_matrix == nullptr || dfa->alphabet_symbols == nullptr) {
        error_code = DFA_MEMORY_ERROR;
        goto failure;
    }


//    memcpy(dfa->alphabet_symbols, symbols, alphabet_size * sizeof(std::string));

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

int get_symbol_index(dfa_t *dfa, const std::string& symbol) {
    assert(dfa != nullptr);

    auto it = std::find(dfa->alphabet_symbols->begin(), dfa->alphabet_symbols->end(), symbol);
    int result = it - dfa->alphabet_symbols->begin();
//    int result = array_index_of(dfa->alphabet_symbols, symbol, dfa->alphabet_size, 1);
    return result >= dfa->alphabet_symbols->size() ? DFA_INVALID_SYMBOL : result;
}

int DFA_run_trace(dfa_t *dfa, const std::vector<std::string>& trace, int length) {

    if (dfa == nullptr || length < 0) {
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
    int *symbol_permutation = nullptr;
    int *matching = nullptr;
    int pattern_states, main_states, main_alphabet_size, pattern_alphabet_size;
    pattern_output_t *output;

    if (main_dfa == nullptr || pattern == nullptr) {
        goto failure;
    }
    pattern_states = pattern->num_states;
    main_states = main_dfa->num_states;
    main_alphabet_size = main_dfa->alphabet_size;
    pattern_alphabet_size = pattern->alphabet_size;

    if (pattern_states > main_states) {
        goto failure;
    }

    symbol_permutation = (int*)calloc(pattern_alphabet_size, sizeof(int));
    matching = (int*)calloc(pattern_states, sizeof(int));
    if (symbol_permutation == nullptr || matching == nullptr) {
        goto failure;
    }
    std::vector<std::string> *out_symbols;

    while(get_next_permutation(matching, pattern_states, main_states - 1) == 0) {

        int reduced_symbol_table[main_alphabet_size][pattern_states];

        for (int i = 0; i < main_alphabet_size; i++) {
            for(int j = 0; j < pattern_states; j++) {
                int index, dest = (main_dfa->transition_matrix)[matching[j] * main_alphabet_size + i];
                if ((index = array_index_of(matching, dest, pattern_states, 1)) >= 0) {
                    reduced_symbol_table[i][j] = index;
                } else {
                    reduced_symbol_table[i][j] = DFA_DUMMY_SYMBOL;
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
            out_symbols = new std::vector<std::string>();
//            out_symbols->resize(pattern_alphabet_size);
            for (int i = 0; i < pattern_alphabet_size; i++) {
                std::string s = main_dfa->alphabet_symbols->at(symbol_permutation[i]);
                out_symbols->push_back(s);
            }
            output = (pattern_output_t*)malloc(sizeof(pattern_output_t));
            if (output == nullptr) {
                goto failure;
            }
            output->states = matching;
            output->symbols = out_symbols;
            free(symbol_permutation);
            return output;

            permutation_failed:
                continue;
        }
    }

    failure:
        free(symbol_permutation);
        free(matching);
        return nullptr;
}

int DFA_modify(dfa_t *main_dfa, dfa_t *original_pattern, dfa_t *target_pattern) {
    if (main_dfa == nullptr || original_pattern == nullptr || target_pattern == nullptr) {
        return DFA_INVALID_ARG;
    }
    if (original_pattern->num_states != target_pattern->num_states ||
            original_pattern->alphabet_size != target_pattern->alphabet_size) {
        return DFA_NOT_YET_IMPL;
    }
    pattern_output_t *pattern = DFA_find_pattern(main_dfa, original_pattern);
    if (pattern == nullptr) {
        return DFA_PATTERN_NOT_FOUND;
    }
    int pattern_states = original_pattern->num_states;
    int pattern_alphabet = original_pattern->alphabet_size;
    int main_alphabet = main_dfa->alphabet_size;

    for(int state_no = 0; state_no < pattern_states; state_no++) {
        int state = pattern->states[state_no];
        for(int symbol_no = 0; symbol_no < pattern_alphabet; symbol_no++) {
            int symbol_ind = get_symbol_index(main_dfa, pattern->symbols->at(symbol_no));
            if (symbol_ind == DFA_INVALID_SYMBOL) {
                return DFA_PATTERN_NOT_FOUND;
            }
            int target = target_pattern->transition_matrix[state_no * pattern_alphabet + symbol_no];
            main_dfa->transition_matrix[state * main_alphabet + symbol_ind] =
                    target == DFA_DUMMY_SYMBOL ? DFA_DUMMY_SYMBOL : pattern->states[target];
        }
    }

    DFA_free_pattern(pattern);
    return 0;
}

int DFA_parallel(dfa_t *dest, dfa_t *dfa_1, dfa_t *dfa_2) {
    if (dest == nullptr || dfa_1 == nullptr || dfa_2 == nullptr) {
        return DFA_INVALID_ARG;
    }
    int err;

    int num_states_1 = dfa_1->num_states;
    int num_states_2 = dfa_2->num_states;
    int new_num_states = num_states_1 * num_states_2;
    int alphabet_size_1 = dfa_1->alphabet_size;
    int alphabet_size_2 = dfa_2->alphabet_size;

    int *new_transition_matrix = nullptr;
    bool *new_final_states = nullptr;

    std::set<std::string> tmp_set;
    tmp_set.insert(dfa_1->alphabet_symbols->begin(), dfa_1->alphabet_symbols->end());
    tmp_set.insert(dfa_2->alphabet_symbols->begin(), dfa_2->alphabet_symbols->end());
    auto *new_alphabet_symbols = new std::vector<std::string>(tmp_set.begin(), tmp_set.end());
    int new_alph_size = new_alphabet_symbols->size();

    new_final_states = (bool*)calloc(new_num_states, sizeof(bool)); // initialise as false
    new_transition_matrix = (int*)malloc(new_num_states * new_alph_size * sizeof(int));

    if (new_final_states == nullptr
        || new_transition_matrix == nullptr) {
        err = DFA_MEMORY_ERROR;
        goto failure;
    }
    for(int s1 = 0; s1 < num_states_1; s1++) {
        for(int s2 = 0; s2 < num_states_2; s2++) {
            for(int symb_ind = 0; symb_ind < new_alph_size; symb_ind++) {
                int s_ind1 = get_symbol_index(dfa_1, new_alphabet_symbols->at(symb_ind));
                int s_ind2 = get_symbol_index(dfa_2, new_alphabet_symbols->at(symb_ind));
                int M1_target = s_ind1 == DFA_INVALID_SYMBOL ? s1 : dfa_1->transition_matrix[s1 * alphabet_size_1 + s_ind1];
                int M2_target = s_ind2 == DFA_INVALID_SYMBOL ? s2 : dfa_2->transition_matrix[s2 * alphabet_size_2 + s_ind2];
                new_transition_matrix[(s1 * num_states_1 + s2) * new_alph_size + symb_ind] =
                        (M1_target == DFA_DUMMY_SYMBOL || M2_target == DFA_DUMMY_SYMBOL) ?
                            DFA_DUMMY_SYMBOL : M1_target * num_states_1 + M2_target;
            }
            if (dfa_1->final_states[s1] && dfa_2->final_states[s2]) {
                new_final_states[s1 * num_states_1 + s2] = true;
            }
        }
    }

    err = DFA_new(dest, new_num_states, new_alph_size, 0,
                new_final_states, *new_alphabet_symbols,
                new_transition_matrix);
    failure:
    free(new_alphabet_symbols);
    free(new_final_states);
    free(new_transition_matrix);
    return err;
}

void DFA_free_pattern(pattern_output_t *pattern) {
    if (pattern == nullptr) {
        return;
    }
    free(pattern->states);
    free(pattern->symbols);
    free(pattern);
}

void DFA_print(dfa_t *dfa, FILE *f) {
    fprintf(f, "Num states: %d; Alphabet size %d\n", dfa->num_states, dfa->alphabet_size);
    fprintf(f, "Initial state: %d\n", dfa->initial_state);
    fprintf(f, "Final state(s): ");
    for(int i = 0; i < dfa->num_states; i++) {
        if (dfa->final_states[i]) fprintf(f, "%d ", i);
    }
    fprintf(f, "\nAlphabet Symbol(s): \n");
    for(int i = 0; i < dfa->alphabet_size; i++) {
        fprintf(f, "%d - %s\n", i, dfa->alphabet_symbols->at(i).c_str());
    }
    fprintf(f, "\nTransition Matrix:\n");
    array_print(dfa->transition_matrix, dfa->num_states, dfa->alphabet_size, f);
}


int DFA_apply_symbol(dfa_t *dfa, int current_state, const std::string& symbol) {
    if (dfa == nullptr || current_state >= dfa->num_states) {
        return DFA_INVALID_ARG;
    }
    int symbol_index;
    if ((symbol_index = get_symbol_index(dfa, symbol)) < 0) {
        return DFA_INVALID_SYMBOL;
    }
    return dfa->transition_matrix[current_state * dfa->alphabet_size + symbol_index];
}


int DFA_clone(dfa_t *source, dfa_t *target) {
    target->num_states = source->num_states;
    target->initial_state = source->initial_state;
    target->alphabet_size = source->alphabet_size;
    target->final_states = (bool*)malloc(source->num_states * sizeof(bool));
    memcpy(target->final_states, source->final_states, source->num_states * sizeof(bool));


//    target->alphabet_symbols = (int*)malloc(source->alphabet_size * sizeof(int));
//    memcpy(target->alphabet_symbols, source->alphabet_symbols, source->alphabet_size * sizeof(int));

    target->alphabet_symbols = new std::vector<std::string>(source->alphabet_symbols->begin(), source->alphabet_symbols->end());

    target->transition_matrix = (int*)malloc((source->num_states * source->alphabet_size) * sizeof(int));
    memcpy(target->transition_matrix, source->transition_matrix, (source->num_states * source->alphabet_size) * sizeof(int));
    return 0;
}




static int get_next_permutation(int *current_permutation, int length, int max_value) {
    assert(current_permutation != nullptr);

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


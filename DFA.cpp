/** @file DFA.c
 *  @brief Source file for DFA structure
 *  @author Brian Wei
 *
 *  Detailed documentation may be found in the header file DFA.h
 */

#include <cassert>
#include <cstdio>
#include <set>
#include "inc/DFA.h"

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
static int get_next_permutation(std::vector<int>& current_permutation, int max_value);

static bool vec_contains_duplicates(std::vector<int>& v);

static int vec_index_of(std::vector<int>& v, int val);

static void vec_2d_print(const std::vector<std::vector<int>>& v, FILE *f);

/* *****     IMPLEMENTATION     ***** */
dfa::dfa(int num_states, int alphabet_size, int initial_state,
         std::vector<bool>& finals, const std::vector<std::string>& symbols,
         const int *transition_matrix) {
    DFA_constructor_helper(num_states, alphabet_size, initial_state,
            finals, symbols, transition_matrix);
}
void dfa::DFA_constructor_helper(int num_states, int alphabet_size, int initial_state,
        std::vector<bool>& finals, const std::vector<std::string>& symbols,
        const int *transition_matrix) {
    assert(initial_state < num_states && initial_state >= 0);

    this->num_states = num_states;
    this->initial_state = initial_state;
    for(int i = 0; i < num_states; i++) {
        if (finals[i]) this->final_states.insert(i);
    }

    this->alphabet_symbols = std::vector<std::string>(symbols);
    this->transition_matrix.resize(num_states);
    for(int state = 0; state < num_states; state++) {
        this->transition_matrix[state].resize(alphabet_size);
        for(int letter = 0; letter < alphabet_size; letter++) {
            int target_state = transition_matrix[state * alphabet_size + letter];
            assert(target_state < num_states && target_state >= -1);
            this->transition_matrix[state][letter] = target_state;
        }
    }
}

int dfa::get_symbol_index(const std::string& symbol) {
    auto it = std::find(this->alphabet_symbols.begin(), this->alphabet_symbols.end(), symbol);
    int result = it - this->alphabet_symbols.begin();
    return result >= this->alphabet_symbols.size() ? DFA_INVALID_SYMBOL : result;
}

int dfa::DFA_run_trace(const std::vector<std::string>& trace) {
    int length = trace.size();
    int current_state = this->initial_state;
    if (current_state < 0 || current_state >= this->num_states) {
        return DFA_INVALID_ARG;
    }
    for (int i = 0; i < length; i++) {
        int symbol_index = this->get_symbol_index(trace[i]);
        if (symbol_index < 0) {
            return DFA_INVALID_ARG;
        }
        if (this->transition_matrix[current_state][symbol_index] < 0) {
            return 0;
        }
    }
    return this->final_states.find(current_state) == this->final_states.end();
}

pattern_output *dfa::DFA_find_pattern(dfa& pattern, int skip_counter) {
    int pattern_states, main_states, main_alphabet_size, pattern_alphabet_size;
    int find_count = 0;
    pattern_output *output;
    pattern_states = pattern.num_states;
    main_states = this->num_states;
    main_alphabet_size = this->alphabet_symbols.size();
    pattern_alphabet_size = pattern.alphabet_symbols.size();

    assert(pattern_states <= main_states);

    auto symbol_permutation = std::vector<int>(pattern_alphabet_size, 0);
    auto matching = std::vector<int>(pattern_states, 0);

    std::vector<std::string> out_symbols;

    while(get_next_permutation(matching, main_states - 1) == 0) {
        int reduced_symbol_table[main_alphabet_size][pattern_states];

        for (int i = 0; i < main_alphabet_size; i++) {
            for(int j = 0; j < pattern_states; j++) {
                int index, dest = (this->transition_matrix)[matching[j]][i];
                if ((index = vec_index_of(matching, dest)) >= 0) {
                    reduced_symbol_table[i][j] = index;
                } else {
                    reduced_symbol_table[i][j] = DFA_DUMMY_SYMBOL;
                }
            }
        }
        while(get_next_permutation(symbol_permutation, main_alphabet_size - 1) == 0) {

            for (int i = 0; i < pattern_alphabet_size; i++) {
                for (int j = 0; j < pattern_states; j++) {
                    int main_dest_val = reduced_symbol_table[symbol_permutation[i]][j];
                    int pattern_dest_val = pattern.transition_matrix[j][i];
                    if (pattern_dest_val != DFA_DUMMY_SYMBOL && main_dest_val != pattern_dest_val) {
                        goto permutation_failed;
                    }
                }
            }

            if (find_count < skip_counter) {
                find_count++;
                goto permutation_failed;
            }

            out_symbols = std::vector<std::string>();
            for (int i = 0; i < pattern_alphabet_size; i++) {
                std::string s = this->alphabet_symbols[symbol_permutation[i]];
                out_symbols.push_back(s);
            }
            output = new pattern_output;
            output->states = std::vector<int>(matching);
            output->symbols = std::vector<std::string>(out_symbols);
            return output;

            permutation_failed:
                continue;
        }

    }
    return nullptr;
}

int dfa::DFA_modify(dfa& original_pattern, dfa& target_pattern, int skips) {
    int original_asize = original_pattern.alphabet_symbols.size();
    int target_asize = target_pattern.alphabet_symbols.size();
    if (original_pattern.num_states != target_pattern.num_states ||
            original_asize != target_asize) {
        return DFA_NOT_YET_IMPL;
    }

    pattern_output *pattern = DFA_find_pattern(original_pattern, skips);
    if (pattern == nullptr) return DFA_PATTERN_NOT_FOUND;

    int pattern_states = original_pattern.num_states;

    for(int state_no = 0; state_no < pattern_states; state_no++) {
        int state = pattern->states[state_no];
        for(int symbol_no = 0; symbol_no < original_asize; symbol_no++) {
            int symbol_ind = get_symbol_index(pattern->symbols[symbol_no]);
            if (symbol_ind == DFA_INVALID_SYMBOL) {
                return DFA_PATTERN_NOT_FOUND;
            }
            int target = target_pattern.transition_matrix[state_no][symbol_no];
            this->transition_matrix[state][symbol_ind] =
                    target == DFA_DUMMY_SYMBOL ? DFA_DUMMY_SYMBOL : pattern->states[target];
        }
    }
    return 0;
}

dfa::dfa(dfa& dfa_1, dfa& dfa_2) {

    int num_states_1 = dfa_1.num_states;
    int num_states_2 = dfa_2.num_states;

    int new_num_states = num_states_1 * num_states_2;

    std::set<std::string> tmp_set;
    tmp_set.insert(dfa_1.alphabet_symbols.begin(), dfa_1.alphabet_symbols.end());
    tmp_set.insert(dfa_2.alphabet_symbols.begin(), dfa_2.alphabet_symbols.end());
    auto new_alphabet_symbols = std::vector<std::string>(tmp_set.begin(), tmp_set.end());
    int new_alph_size = new_alphabet_symbols.size();
    auto new_final_states = std::vector<bool>( new_num_states, false);
    auto new_transition_matrix = std::vector<int>();
    new_transition_matrix.resize(new_num_states * new_alph_size);

    for(int s1 = 0; s1 < num_states_1; s1++) {
        for(int s2 = 0; s2 < num_states_2; s2++) {
            for(int symb_ind = 0; symb_ind < new_alph_size; symb_ind++) {
                int s_ind1 = dfa_1.get_symbol_index(new_alphabet_symbols[symb_ind]);
                int s_ind2 = dfa_2.get_symbol_index(new_alphabet_symbols[symb_ind]);
                int M1_target = s_ind1 == DFA_INVALID_SYMBOL ? s1 : dfa_1.transition_matrix[s1][s_ind1];
                int M2_target = s_ind2 == DFA_INVALID_SYMBOL ? s2 : dfa_2.transition_matrix[s2][s_ind2];
                new_transition_matrix[(s1 * num_states_2 + s2) * new_alph_size + symb_ind] =
                        (M1_target == DFA_DUMMY_SYMBOL || M2_target == DFA_DUMMY_SYMBOL) ?
                            DFA_DUMMY_SYMBOL : (M1_target * num_states_2 + M2_target);
            }
            if (dfa_1.final_states.count(s1) && dfa_2.final_states.count(s2)) {
                new_final_states[s1 * num_states_1 + s2] = true;
            }
        }
    }

    DFA_constructor_helper(new_num_states, new_alph_size, 0,
                new_final_states, new_alphabet_symbols,
                new_transition_matrix.data());
}

void dfa::DFA_print(FILE *f) const {
    int alphabet_size = this->alphabet_symbols.size();
    fprintf(f, "Num states: %d; Alphabet size %d\n", this->num_states, alphabet_size);
    fprintf(f, "Initial state: %d\n", this->initial_state);
    fprintf(f, "Final state(s): ");
    for(int state : this->final_states) {
        fprintf(f, "%d ", state);
    }
    fprintf(f, "\nAlphabet Symbol(s): \n");
    for(int i = 0; i < alphabet_size; i++) {
        fprintf(f, "%d - %s\n", i, this->alphabet_symbols[i].c_str());
    }
    fprintf(f, "\nTransition Matrix:\n");
    vec_2d_print(this->transition_matrix, f);
}

int dfa::DFA_apply_symbol(int current_state, const std::string& symbol) {
    assert(this->num_states > current_state);
    int symbol_index;
    if ((symbol_index = this->get_symbol_index(symbol)) < 0) {
        return DFA_INVALID_SYMBOL;
    }
    return this->transition_matrix[current_state][symbol_index];
}

dfa::dfa(dfa& source) {
    this->num_states = source.num_states;
    this->initial_state = source.initial_state;
    this->final_states = std::set<int>(source.final_states);
    this->alphabet_symbols = std::vector<std::string>(source.alphabet_symbols);
    this->transition_matrix = std::vector<std::vector<int>>();
    this->transition_matrix.resize(source.transition_matrix.size());
    for(int i = 0; i < source.transition_matrix.size(); i++) {
        this->transition_matrix[i] = std::vector<int>(source.transition_matrix[i]);
    }
}

static void vec_2d_print(const std::vector<std::vector<int>>& v, FILE *f) {
    for(const auto & i : v) {
        for(int j : i) {
            fprintf(f, "%d ", j);
        }
        fprintf(f, "\n");
    }
}


static bool vec_contains_duplicates(std::vector<int>& v) {
    std::set<int> s(v.begin(), v.end());
    return s.size() != v.size();
}

static int vec_index_of(std::vector<int>& v, int val) {
    auto it = std::find(v.begin(), v.end(), val);
    if (it == v.end()) return -1;
    else return std::distance(v.begin(), it);
}

static int get_next_permutation(std::vector<int>& current_permutation, int max_value) {
    int length = current_permutation.size();

    start_of_loop:
    for(int i = length - 1; i >= 0; i--) {
        int new_val = current_permutation[i]++;
        if (new_val == max_value) {
            current_permutation[i] = 0;
            continue;
        }
        if (vec_contains_duplicates(current_permutation)) {
            goto start_of_loop;
        } else {
            return 0;
        }
    }
    return -1;
}


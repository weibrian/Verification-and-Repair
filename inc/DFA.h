#ifndef __VERIF_DFA_H__
#define __VERIF_DFA_H__

#include <stdbool.h>

#define INVALID_SYMBOL (-1)

typedef struct dfa {
  int num_states;           /* Number of states */
  int initial_state;        /* Initial state    */
  bool *final_states;       /* Accepting states */
  int alphabet_size;        /* Size of alphabet */
  int *alphabet_symbols;    /* Symbols in the alphabet  */
  int *transition_matrix;   /* Transition matrix of num_states * alphabet_size
                             * where tm[i][j] is the destination from state i
                             * on transition alphabet_symbols[j] */
} dfa_t;

int DFA_new(dfa_t *dfa, int num_states, int alphabet_size, int initial_state,
            const bool finals[num_states], int symbols[alphabet_size],
            int transition_matrix[num_states][alphabet_size]);

int get_symbol_index(dfa_t *dfa, int symbol);

int DFA_run_trace(dfa_t *dfa, const int *trace, int length);

int *DFA_find_pattern(dfa_t *main_dfa, dfa_t *pattern);



#endif /* __VERIF_DFA_H__ */

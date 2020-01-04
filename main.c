#include <stdio.h>
#include "inc/DFA.h"

int main() {
    dfa_t main_dfa;
    bool main_finals[4] = { false, false, false, false };
    int symbols[2] = {44, 55};
    int transitions[4][2] = {{1,3},{2,0},{3,1},{0,2}};
    DFA_new(&main_dfa, 4, 2, 0, main_finals, symbols, transitions);

    dfa_t pattern_dfa;
    bool pattern_finals[3] = {false, false, false};
    int p_symbs[2] = {66, 77};
    int p_trans[3][2] = {{1,-1},{2,0},{-1,1}};
    DFA_new(&pattern_dfa, 3, 2, 0, pattern_finals, p_symbs, p_trans);
    printf("DFA initialization complete.\n");

    DFA_find_pattern(&main_dfa, &pattern_dfa);
    return 0;
}
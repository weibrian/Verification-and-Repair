#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/DFA.h"
#include "inc/array_util.h"
#include "inc/examples.h"

int main() {
    dfa_t main_dfa, pattern1, pattern2;
    if(ex_cdm(&main_dfa) < 0 || ex_sink(&pattern1) || ex_linear(&pattern2)) {
        printf("DFA initialization error\n");
    }

    DFA_print(&main_dfa, stdout);
    int flag = DFA_modify(&main_dfa, &pattern1, &pattern2);
    printf("DFA modify exited with status %d\n", flag);
    DFA_print(&main_dfa, stdout);
 /*   pattern_output_t *identified_pattern = DFA_find_pattern(&main_dfa, &pattern);
    if (identified_pattern != NULL) {
        array_print(identified_pattern->symbols, 1, 2);
        array_print(identified_pattern->states, 1, 3);
    } else {
        printf("No matching found\n");
    }

    DFA_free_pattern(identified_pattern);
   */ return 0;
}
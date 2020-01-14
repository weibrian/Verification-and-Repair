#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/DFA.h"
#include "inc/array_util.h"
#include "inc/examples.h"

int main() {
    dfa_t main_dfa, pattern;
    if(ex_cdm(&main_dfa) < 0 || ex_sink(&pattern)) {
        printf("DFA initialization error\n");
    }

    pattern_output_t *identified_pattern = DFA_find_pattern(&main_dfa, &pattern);
    if (identified_pattern != NULL) {
        array_print(identified_pattern->symbols, 1, 2);
        array_print(identified_pattern->states, 1, 3);
    } else {
        printf("No matching found\n");
    }
    return 0;
}
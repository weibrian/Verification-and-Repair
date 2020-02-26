//
// Created by Brian Wei on 2/26/20.
//

#include <fstream>
#include <iostream>
#include <cstdio>
#include <map>
#include "inc/ltsa_parser.h"
#include "inc/array_util.h"


static void ignore_lines(std::string &line, std::ifstream &f, int count) {
    for (int i = 0; i < count; i++) {
        std::getline(f, line);
    }
}

int parser_go(dfa_t *dfa, const char *path) {
    std::string line;
    std::ifstream f(path);
    ignore_lines(line, f, 3);
    int num_states;
    f >> num_states;
    ignore_lines(line, f, 3);
    int current_state_number = 0;
    int current_transition_counter = 0;

    std::map<std::string, int> transition_numbers;
    std::map<std::string, int>::iterator it;

    std::map<int, int> transitions[num_states];

    while (std::getline(f, line)) {
        int ind_of_arrow = line.find("->", 0);
        int start_of_trans_name;
        if ((start_of_trans_name = line.find('(', 0)) != std::string::npos) {
            start_of_trans_name++;
        } else if ((start_of_trans_name = line.find('|', 0)) != std::string::npos) {
            start_of_trans_name++;
        } else {
           return -1;
        }
        int trans_name_len = ind_of_arrow - start_of_trans_name;
        std::string trans_name = line.substr(start_of_trans_name, trans_name_len);
        int this_transition_num;
        if ((it = transition_numbers.find(trans_name)) == transition_numbers.end()) {
            transition_numbers.insert(std::pair<std::string, int>(trans_name, current_transition_counter));
            this_transition_num = current_transition_counter;
            current_transition_counter++;
        } else {
            this_transition_num = it->second;
        }
        const char *ending = line.substr(1 + line.find_last_of('Q', std::string::npos)).c_str();
        int target_state;
        sscanf(ending, "%d", &target_state);
        transitions[current_state_number].insert(std::pair<int, int>(this_transition_num, target_state));

        if (line.find(',', 0) != std::string::npos) {
            current_state_number++;
        }
    }
    bool finals[num_states];
    memset(finals, 0, num_states * sizeof(bool));
    int alphabet_size = current_transition_counter - 1;
    int *trans = (int *) malloc(num_states * alphabet_size * sizeof(int));
    if (trans == nullptr) {
        return -1;
    }
    int symbols[alphabet_size];
    for (int i = 0; i < alphabet_size; i++) {
        symbols[i] = i;
    }
    for (int q = 0; q < num_states; q++) {
        for (int s = 0; s < alphabet_size; s++) {
            std::map<int, int>::iterator it;
            it = transitions[q].find(s);
            if (it == transitions[q].end()) continue;
            else trans[q * alphabet_size + s] = it->second;
        }
    }
    int out = DFA_new(dfa, num_states, alphabet_size, 0, finals, symbols, trans);
    free(trans);
    return out;
}

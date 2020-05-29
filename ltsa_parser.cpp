/** @file ltsa_parser.cpp
 *  @brief Parser for LTSA output files
 *  @author Brian Wei
 *
 *  See header file for detailed documentation
 */

#include <fstream>
#include <iostream>
#include <cstdio>
#include <map>
#include <string>
#include <cctype>
#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include "inc/ltsa_parser.h"

/* strip whitespace string */
#define STRIP(str) ((str).erase(std::remove_if((str).begin(), (str).end(), ::isspace), (str).end()));

/** @brief Ignore some lines from a file
 *
 * @param line Probably not needed...
 * @param f File stream to ignore from
 * @param count Number of lines to ignore
 */
static void ignore_lines(std::string &line, std::ifstream &f, int count);

/* ***** IMPLEMENTATIONN ***** */

static void ignore_lines(std::string &line, std::ifstream &f, int count) {
    for (int i = 0; i < count; i++) {
        std::getline(f, line);
    }
}

dfa *parser_go(const char *path) {

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
    std::vector<int> sinks;
    auto *symbols = new std::vector<std::string>();
    while (std::getline(f, line)) {
        int ind_of_arrow = line.find("->", 0);
        int start_of_trans_name;
        if ((start_of_trans_name = line.find('(', 0)) != std::string::npos) {
            start_of_trans_name++;
        } else if ((start_of_trans_name = line.find('|', 0)) != std::string::npos) {
            start_of_trans_name++;
        } else {
            assert(line.find("STOP", 0) != std::string::npos);
            sinks.push_back(current_state_number++);
            continue;
        }
        const char *ending = line.substr(1 + line.find_last_of('Q', std::string::npos)).c_str();
        int target_state;
        sscanf(ending, "%d", &target_state);

        std::pair<int, int> curly_inds;
        curly_inds.first = line.find('{', 0);
        curly_inds.second = line.find('}', 0);
        if (curly_inds.first != std::string::npos && curly_inds.second != std::string::npos) {
            std::string between_curly = line.substr(curly_inds.first + 1, curly_inds.second - curly_inds.first - 1);
            std::vector<std::string> tokens;
            boost::split(tokens, between_curly, boost::is_any_of(","));
            for (auto& t : tokens) {
                STRIP(t);
                int this_transition_num;
                if ((it = transition_numbers.find(t)) == transition_numbers.end()) {
                    transition_numbers.insert(std::pair<std::string, int>(t, current_transition_counter));
                    this_transition_num = current_transition_counter;
                    current_transition_counter++;
                    symbols->push_back(t);
                } else {
                    this_transition_num = it->second;
                }
                transitions[current_state_number].insert(std::pair<int, int>(this_transition_num, target_state));
            }
        } else {
            int trans_name_len = ind_of_arrow - start_of_trans_name;
            std::string trans_name = line.substr(start_of_trans_name, trans_name_len);
            STRIP(trans_name);
            int this_transition_num;
            if ((it = transition_numbers.find(trans_name)) == transition_numbers.end()) {
                transition_numbers.insert(std::pair<std::string, int>(trans_name, current_transition_counter));
                this_transition_num = current_transition_counter;
                current_transition_counter++;
                symbols->push_back(trans_name);
            } else {
                this_transition_num = it->second;
            }

            transitions[current_state_number].insert(std::pair<int, int>(this_transition_num, target_state));
        }
        if (line.at(line.length() - 1) == ',') {
            current_state_number++;
        }
    }

    std::vector<bool> finals(num_states, false);
    int alphabet_size = current_transition_counter;
    std::vector<int> trans;
    trans.resize(num_states * alphabet_size);

    for (int q = 0; q < num_states; q++) {
        if (std::find(sinks.begin(), sinks.end(), q) != sinks.end()) {
            for (int s = 0; s < alphabet_size; s++) {
                trans[q * alphabet_size + s] = q;
            }
            continue;
        }
        for (int s = 0; s < alphabet_size; s++) {
            auto it = transitions[q].find(s);
            if (it == transitions[q].end()) trans[q * alphabet_size + s] = -1;
            else trans[q * alphabet_size + s] = it->second;
        }
    }
    return new dfa(num_states, alphabet_size, 0,
            finals, *symbols, trans.data());
}

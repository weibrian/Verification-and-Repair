/** @file examples.c
 *  @brief Example state machines and patterns
 *  @author Brian Wei
 *
 *  Detailed documentation may be found in the header file examples.h
 */

#include "inc/examples.h"
#include "inc/ltsa_parser.h"

dfa *ex_cycle(int size) {
    assert(size > 0);
    auto *symbols = new std::vector<std::string>({"A", "B"});

    std::vector<bool> main_finals(size, false);
    std::vector<int> transitions;
    transitions.resize(size * 2);
    for (int i = 0; i < size; i++) {
        transitions[i * 2] = (i + 1) % size;
        transitions[i * 2 + 1] = (i - 1) % size;
    }
    return new dfa(size, 2, 0,
            main_finals, *symbols, transitions.data());
}

dfa *ex_cdm() {
    std::vector<bool> finals(6, false);
    auto *transitions = new int[12] {1,1,2,3,4,5,5,4,5,5,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL};
    auto *symbols = new std::vector<std::string>({"A", "B"});

    return new dfa(6, 2, 0,
            finals, *symbols, transitions);
}

/* Patterns */

dfa *ex_linear() {
    const int NUM_STATES = 3;
    std::vector<bool> finals(NUM_STATES, false);
    auto *transitions = new int[2 * NUM_STATES] {1,1,2,2,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL};
    auto *symbols = new std::vector<std::string>({"A", "B"});

    return new dfa(NUM_STATES, 2, 0,
            finals, *symbols, transitions);
}
dfa *ex_twoway() {
    const int NUM_STATES = 3;
    std::vector<bool> finals(NUM_STATES, false);
    auto *transitions = new int[2 * NUM_STATES] {1,DFA_DUMMY_SYMBOL,2,0,DFA_DUMMY_SYMBOL,1};
    auto *symbols = new std::vector<std::string>({"A", "B"});

    return new dfa(NUM_STATES, 2, 0,
                   finals, *symbols, transitions);
}

dfa *ex_sink() {
    const int NUM_STATES = 3;
    std::vector<bool> finals(NUM_STATES, false);
    auto *transitions = new int[2 * NUM_STATES]  {1,2,2,2,DFA_DUMMY_SYMBOL,DFA_DUMMY_SYMBOL};
    auto *symbols = new std::vector<std::string>({"A", "B"});

    return new dfa(NUM_STATES, 2, 0,
                   finals, *symbols, transitions);
}


dfa *ex_fancy() {
    const int NUM_STATES = 3;
    std::vector<bool> finals(NUM_STATES, false);
    auto *transitions = new int[2 * NUM_STATES] {1,2,2,2,0,0};
    auto *symbols = new std::vector<std::string>({"A", "B"});

    return new dfa(NUM_STATES, 2, 0,
                   finals, *symbols, transitions);
}


dfa *prop_five() {
    const int NUM_STATES = 6;
    std::vector<bool> finals(NUM_STATES, false);
    auto *transitions = new int[NUM_STATES]{1, 2, 3, 4, 5, DFA_DUMMY_SYMBOL};
    auto *symbols = new std::vector<std::string>({"A"});

    return new dfa(NUM_STATES, 1, 0,
                   finals, *symbols, transitions);
}

dfa *ex_infusion() {
    return parser_go("./ltsa_models/infusion_pump_relaxed");
}

dfa *ex_human() {
    return parser_go("./ltsa_models/infusion_pump_human");
}

dfa *ex_prop() {
    return parser_go("./ltsa_models/infusion_pump_amtchecker");
}
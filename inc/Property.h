/** @file Property.h
 *  @brief Header for properties
 *  @author Brian Wei
 *
 *  This serves as an implementation of properties, including the key
 *  functionality of checking whether a dfa satisfies a property
 */

#ifndef __VERIF_PROPERTY_H__
#define __VERIF_PROPERTY_H__

#include "DFA.h"

typedef enum class interps { NOP, ERROR } interps_t;

class Property {
private:
    dfa *sim_dfa; /* DFA simulating the property */
    interps_t invalid_interp; /* Mode to interpret */
    std::set<int> error_states; /* states which represent errors */
//    int *error_states; /* states which represent errors */
//    int num_error_states; /* number of error states */
public:

    /** @brief Constructor for a property
     *
     * @param dfa State machine representing the property
     * @param mode Mode to interpret the output
     * @param error_states List of error states
     * @param num_error_states number of error states, should be length of error_states
     */
    Property(dfa& dfa, interps_t mode, int *error_states, int num_error_states);

    /** @brief Print the details of a property to standard out
     */
    void property_print();

    /** @brief Checks if a DFA satisfies the property
     *
     * @param dfa State machine to check the property on
     * @return True if the property is satisfied, false if not
     */
    bool property_check(dfa &M);
};


#endif /* __VERIF_PROPERTY_H__ */

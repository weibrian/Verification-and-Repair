/** @file examples.h
 *  @brief Example state machines and patterns
 *  @author Brian Wei
 *
 *  Various examples of Finite State Machines and patterns for
 *  demonstrative and testing purposes
 */
#ifndef __VERIF_EXAMPLES_H__
#define __VERIF_EXAMPLES_H__

#include "DFA.h"

/* State Machines */

/** @brief Generates a cycles of two symbols with transitions in opposite directions
 *
 *   e.g. size = 4
 *
 *      +---+           +---+           +---+           +---+
 *      | 0 |-(a->,b<-)-| 1 |-(a->,b<-)-| 2 |-(a->,b<-)-| 3 |
 *      +---+           +---+           +---+           +---+
 *        |                                               |
 *        +-----------------(a<-)---(b->)-----------------+
 *
 *  @param dfa DFA to load into
 *  @param size Number of states
 *  @return 0 on success, negative error code on failure
 */
int ex_cycle(dfa_t *dfa, int size);

/** @brief Generates a simple six state DFA
 *
 *        +---+
 *        | 0 |--a,b->-+
 *        +---+        |
 *                     |
 *       +---+       +---+       +---+
 *     +-| 2 |--<-a--| 1 |--b->--| 3 |--a->-+
 *     | +---+       +---+       +---+      |
 *     |   |                       |        |
 *     |   |         +---+         |  +-----+
 *     |   +----a->--| 4 |--<-b----+  |
 *     |             +---+            |
 *     |               |              |       +--a,b->--+
 *     |              a,b             |       |         |
 *     |               |              |     +---+       |
 *     +--------b->----+------>-------+-->--| 5 |--<----+
 *                                          +---+
 *
 * @param dfa DFA to load into
 * @return 0 on success, negative error code on failure
 */
int ex_cdm(dfa_t *dfa);

/* Patterns */

/** @brief Three state pattern with two symbols linear
 *
 *     +---+         +---+         +---+
 *     | 0 |-(a,b)-->| 1 |-(a,b)-->| 2 |
 *     +---+         +---+         +---+
 *
 * @param dfa DFA to load into
 * @return 0 on success, negative error code on failure
 */
int ex_linear(dfa_t *dfa);

/** @brief Three state patter with two symbols, bidirectional
 *
 *     +---+             +---+             +---+
 *     | 0 |--(a->,b<-)--| 1 |--(a->,b<-)--| 2 |
 *     +---+             +---+             +---+
 *
 * @param dfa DFA to load into
 * @return 0 on success, negative error code on failure
 */
int ex_twoway(dfa_t *dfa);

/** @brief Three state pattern with two symbols and a sink state
 *
 *     +---+       +---+         +---+
 *     | 0 |-(a)-->| 1 |-(a,b)-->| 2 |
 *     +---+       +---+         +---+
 *       |                         |
 *       +---------(b)--->---------+
 *
 * @param dfa DFA to load into
 * @return 0 on success, negative error code on failure
 */
int ex_sink(dfa_t *dfa);

#endif /* __VERIF_EXAMPLES_H__ */

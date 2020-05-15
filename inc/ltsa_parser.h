/** @file ltsa_parser.h
 *  @brief Header file for LTSA parser
 *  @author Brian Wei
 *
 *  Parses the output format created by the LTSA tool into the DFA structure
 */

#ifndef __VERIF_LTSA_PARSER_H__
#define __VERIF_LTSA_PARSER_H__

#include "DFA.h"

/** @brief Parses the LTSA output into dfa
 *
 * @param dfa DFA to put the output into
 * @param path Relative path to the LTSA output file
 * @return zero on success, negative error code on failure
 */
int parser_go(dfa_t *dfa, const char *path);

#endif /* __VERIF_LTSA_PARSER_H__ */

/** @file pattern_lib.h
 *  @brief Header for Pattern Library
 *  @author Brian Wei
 *
 *  This file implements the pattern library.  This contains various mappings
 *  of patterns of real world errors.
 */

#ifndef __VERIF_PATTERN_LIB_H__
#define __VERIF_PATTERN_LIB_H__

#include "modify.h"

/** @brief Initializes the pattern library
 *
 * Current implementation of the pattern library includes premature start, delayed
 * start, omission, reversal, intrusion, and repetition
 *
 * @param mappings Reference to mapping_list to put the pattens in
 * @return zero on success, negative error code on failure
 */
int patternlib_init(mapping_list &mappings);

#endif /* __VERIF_PATTERN_LIB_H__ */


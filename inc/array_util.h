#ifndef __VERIF_ARRAY_UITL_H__
#define __VERIF_ARRAY_UITL_H__

#include <stdbool.h>

bool array_contains_duplicates(const int *array, int length);
bool array_contains(const int *array, int target, int length);
void array_print(const int *array, int rows, int cols, FILE *f);
int array_index_of(const int *array, int target, int length, int instance);
void array_quicksort(int *array, int length);
int array_union(const int *a1, int a1_length, const int *a2, int a2_length, int *out);

#endif /* __VERIF_ARRAY_UITL_H__ */

#include <stdio.h>
#include <stdbool.h>
#include "inc/array_util.h"

bool array_contains(const int *array, int target, int length) {
    if (array == NULL) return false;
    for (int i = 0; i < length; i++) {
        if (array[i] == target) return true;
    }
    return false;
}

bool array_contains_duplicates(const int *array, int length) {
    for(int i = 0; i < length; i++) {
        for(int j = 0; j < length; j++) {
            if (i != j && array[i] == array[j]) {
                return true;
            }
        }
    }
    return false;
}


void array_print(const int *array, int rows, int cols, FILE *f) {

    for(int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(f, "%d ", array[i * cols + j]);
        }
        fprintf(f, "\n");
    }
}

int array_index_of(const int *array, int target, int length, int instance) {
    if (array == NULL) return -1;
    for (int i = 0; i < length; i++) {
        if (array[i] == target) {
            instance--;
            if (instance == 0) {
                return i;
            }
        }
    }
    return -1;
}
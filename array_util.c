#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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

int array_union(const int *a1, int a1_length, const int *a2, int a2_length, int *out) {
    int *a1_copy = NULL, *a2_copy = NULL;
    a1_copy = malloc(a1_length * sizeof(int));
    a2_copy = malloc(a2_length * sizeof(int));
    if (a1_copy == NULL || a2_copy == NULL) {
        free(a1_copy);
        free(a2_copy);
        return -1;
    }
    memcpy(a1_copy, a1, a1_length * sizeof(int));
    memcpy(a2_copy, a2, a2_length * sizeof(int));
    array_quicksort(a1_copy, a1_length);
    array_quicksort(a2_copy, a2_length);
    int i = 0, j = 0, elem_count = 0;
    while(i != a1_length && j != a2_length) {
        if (a1_copy[i] < a2_copy[j]) {
            out[elem_count++] = a1_copy[i++];
        } else if (a1_copy[i] == a2_copy[j]) {
            out[elem_count++] = a1_copy[i++];
            j++;
        } else { // a1_copy[i] > a2_copy[j]
            out[elem_count++] = a2_copy[j++];
        }
    }
    while(i != a1_length) {
        out[elem_count++] = a1_copy[i++];
    }
    while(j != a2_length) {
        out[elem_count++] = a2_copy[j++];
    }
    free(a1_copy);
    free(a2_copy);
    return elem_count;
}

void array_print(const int *array, int rows, int cols, FILE *f) {

    for(int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(f, "%d ", array[i * cols + j]);
        }
        fprintf(f, "\n");
    }
}

void array_quicksort(int *array, int length) {
    if (length <= 2) return;
    int pivot = array[length/2];
    int i,j;
    for(i = 0, j = length - 1;; i++, j--) {
        while(array[i] < pivot) i++;
        while(array[j] > pivot) j--;
        if (i >= j) break;
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
    array_quicksort(array, i);
    array_quicksort(array + i, length - i);
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
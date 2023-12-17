#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"

void swap(int *arr, int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void initializeArray(int *arr, int n)
{
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
}

void shuffle(int *arr, int n) 
{
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(arr, i, j);
    }
}

void printArray(int *arr, int n) 
{
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n");
}

int checkArray(int *arr, int n) 
{
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    
    return 1;
}

int *merge(int *arr1, int n1, int *arr2, int n2) {
    int *result = malloc((n1 + n2) * sizeof(int));
    if (result == NULL) {
        exit(EXIT_FAILURE);
    }
    int i = 0, j = 0, k = 0;

    while (i < n1 && j < n2) {
        if (arr1[i] < arr2[j]) {
            result[k++] = arr1[i++];
        } else {
            result[k++] = arr2[j++];
        }
    }

    while (i < n1) {
        result[k++] = arr1[i++];
    }
    while (j < n2) {
        result[k++] = arr2[j++];
    }

    return result;
} 

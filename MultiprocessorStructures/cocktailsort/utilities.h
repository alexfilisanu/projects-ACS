#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define NUM_ELEMENTS 100000
#define NUM_THREADS 4

struct ThreadArgs {
    int *arr;
    int start;
    int end;
};

void swap(int *arr, int i, int j);
void initializeArray(int *arr, int n);
void shuffle(int *arr, int n);
void printArray(int *arr, int n);
int checkArray(int *arr, int n);
int *merge(int *arr1, int n1, int *arr2, int n2);

#endif // UTILITIES_H

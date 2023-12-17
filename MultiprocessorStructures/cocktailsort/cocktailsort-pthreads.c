#include <pthread.h>
#include "utilities.h"

void *cocktailSort(void *arg)
{
    struct ThreadArgs *args = (struct ThreadArgs*)arg;
    int *a = args->arr;
    int start = args->start;
    int end = args->end;

    int swapped = 1;

    while (swapped) {
        swapped = 0;

        for (int i = start; i < end; i++) {
            if (a[i] > a[i + 1]) {
                swap(a, i, i + 1);
                swapped = 1;
            }
        }

        if (!swapped) {
            break;
        }
        swapped = 0;
        end--;

        for (int i = end - 1; i >= start; i--) {
            if (a[i] > a[i + 1]) {
                swap(a, i, i + 1);
                swapped = 1;
            }
        }

        start++;
    }

    pthread_exit(NULL);
}

void parallelCocktailSort(int *arr, int n, int num_threads)
{
    pthread_t threads[num_threads];
    struct ThreadArgs args[num_threads];

    int chunk_size = n / num_threads;

    for (int i = 0; i < num_threads; i++) {
        args[i].arr = arr;
        args[i].start = i * chunk_size;
        args[i].end = (i == num_threads - 1) ? n - 1 : (i + 1) * chunk_size - 1;

        pthread_create(&threads[i], NULL, cocktailSort, (void*)&args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 1; i < num_threads; i++) {
        int mid = i * chunk_size - 1;
        int *arr1 = arr;
        int n1 = mid + 1;
        int *arr2 = arr + (i * chunk_size);
        int n2 = (i == num_threads - 1) ? (n - n1) : chunk_size;

        int *merged = merge(arr1, n1, arr2, n2);

        for (int j = 0; j < n1 + n2; ++j) {
            arr[j] = merged[j];
        }

        free(merged);
    }
}

int main()
{
    struct timeval start_time, end_time;
    int *a = malloc(NUM_ELEMENTS * sizeof(int));
    if (a == NULL) {
        exit(EXIT_FAILURE);
    }
    initializeArray(a, NUM_ELEMENTS);	
    
    srand((unsigned int)time(NULL));
    shuffle(a, NUM_ELEMENTS);
    
    gettimeofday(&start_time, NULL);
    parallelCocktailSort(a, NUM_ELEMENTS, NUM_THREADS);
    gettimeofday(&end_time, NULL);

    if (checkArray(a, NUM_ELEMENTS)) {
        printf("The array is sorted.\n");
    } else {
        printf("The array is not sorted.\n");
    }

    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("Elapsed time: %.6f seconds\n", elapsed_time);
    
    free(a);
    return 0;
}

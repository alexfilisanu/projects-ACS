#include "utilities.h"

void cocktailSort(int *a, int n)
{
    int swapped = 1;
    int start = 0;
    int end = n - 1;

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
    cocktailSort(a, NUM_ELEMENTS);
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

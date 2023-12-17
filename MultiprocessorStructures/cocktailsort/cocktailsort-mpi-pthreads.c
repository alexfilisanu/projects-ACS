#include <mpi.h>
#include <pthread.h>
#include "utilities.h"

void *cocktailSort(void *arg)
{
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
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

        pthread_create(&threads[i], NULL, cocktailSort, (void *)&args[i]);
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

int main(int argc, char *argv[])
{
    int *a = NULL;
    int number_of_elements;
    double elapsed_time;

    int rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS) {
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    int number_of_processes, rank_of_process;
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_of_process);

    if (rank_of_process == 0) {
        number_of_elements = NUM_ELEMENTS;
        a = malloc(number_of_elements * sizeof(int));
        if (a == NULL) {
            exit(EXIT_FAILURE);
        }

        initializeArray(a, NUM_ELEMENTS);

        srand((unsigned int)time(NULL));
        shuffle(a, NUM_ELEMENTS);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    elapsed_time -= MPI_Wtime();

    MPI_Bcast(&number_of_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int chunk_size = (number_of_elements + number_of_processes - 1) / number_of_processes;
    int *chunk = malloc(chunk_size * sizeof(int));
    if (chunk == NULL) {
        exit(EXIT_FAILURE);
    }

    MPI_Scatter(a, chunk_size, MPI_INT, chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    free(a);
    a = NULL;

    int own_chunk_size = (number_of_elements >= chunk_size * (rank_of_process + 1))
                             ? chunk_size
                             : (number_of_elements - chunk_size * rank_of_process);

    parallelCocktailSort(chunk, own_chunk_size, NUM_THREADS);

    for (int step = 1; step < number_of_processes; step = 2 * step) {
        if (rank_of_process % (2 * step) != 0) {
            MPI_Send(chunk, own_chunk_size, MPI_INT, rank_of_process - step, 0, MPI_COMM_WORLD);
            break;
        }

        if (rank_of_process + step < number_of_processes) {
            int received_chunk_size = (number_of_elements >= chunk_size * (rank_of_process + 2 * step))
                                         ? (chunk_size * step)
                                         : (number_of_elements - chunk_size * (rank_of_process + step));
            int *chunk_received = malloc(received_chunk_size * sizeof(int));
            if (chunk_received == NULL) {
                exit(EXIT_FAILURE);
            }

            MPI_Recv(chunk_received, received_chunk_size, MPI_INT, rank_of_process + step, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            a = merge(chunk, own_chunk_size, chunk_received, received_chunk_size);

            free(chunk);
            free(chunk_received);
            chunk = a;
            own_chunk_size = own_chunk_size + received_chunk_size;
        }
    }

    elapsed_time += MPI_Wtime();

    if (rank_of_process == 0) {
        if (checkArray(chunk, NUM_ELEMENTS)) {
            printf("The array is sorted.\n");
        } else {
            printf("The array is not sorted.\n");
        }

        printf("Elapsed time: %.6f seconds\n", elapsed_time);
    }

    MPI_Finalize();
    return 0;
}

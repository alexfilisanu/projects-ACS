#include <mpi.h>
#include <omp.h>
#include "utilities.h"

void cocktailSort(int *a, int n) {
    int swapped = 1;
    int start = 0;
    int end = n - 1;

    while (swapped) {
        swapped = 0;

        #pragma omp parallel for shared(a, swapped)
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

        #pragma omp parallel for shared(a, swapped)
        for (int i = end - 1; i >= start; i--) {
            if (a[i] > a[i + 1]) {
                swap(a, i, i + 1);
                swapped = 1;
            }
        }

        start++;
    }
}

int main(int argc, char *argv[]) {
    int *a = NULL;
    int number_of_elements;
    double elapsed_time;

    int rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS) {
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    int number_of_process, rank_of_process;
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_process);
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

    int chunk_size = (number_of_elements + number_of_process - 1) / number_of_process;
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

    #pragma omp parallel
    {
        cocktailSort(chunk, own_chunk_size);
    }

    for (int step = 1; step < number_of_process; step = 2 * step) {
        if (rank_of_process % (2 * step) != 0) {
            MPI_Send(chunk, own_chunk_size, MPI_INT, rank_of_process - step, 0, MPI_COMM_WORLD);
            break;
        }

        if (rank_of_process + step < number_of_process) {
            int received_chunk_size =
                (number_of_elements >= chunk_size * (rank_of_process + 2 * step))
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
    free(chunk);
    return 0;
}

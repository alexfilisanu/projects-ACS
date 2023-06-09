/*
 * Tema 2 ASC
 * 2023 Spring
 */
#include "utils.h"

/*
 * Add your unoptimized implementation here
 */
void multiply_regular_with_regular(int N, double *A, double *B, double *result) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            result[i * N + j] = sum;
        }
    }
}

void multiply_upper_with_regular(int N, double *A, double *B, double *result) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = i; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            result[i * N + j] = sum;
        }
    }
}

void multiply_regular_with_lower(int N, double *A, double *B, double *result) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = j; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            result[i * N + j] = sum;
        }
    }
}

void add_two_matrices(int N, double *A, double *B, double *result) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i * N + j] = A[i * N + j] + B[i * N + j];
        }
    }
}

void transpose_regular(int N, double *A, double *result) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i * N + j] = A[j * N + i];
        }
    }
}

void transpose_upper(int N, double *A, double *result) {
     for (int i = 0; i < N; i++) {
        for (int j = i; j < N; j++) {
            result[j * N + i] = A[i * N + j];
        }
    }
}

double* my_solver(int N, double *A, double* B) {
    double *result = calloc(N * N, sizeof(double));
    if (!result) {
		return NULL;	
    }

    double *A_transpose = calloc(N * N, sizeof(double));
    if (!A_transpose) {
		return NULL;	
    }

    double *B_transpose = calloc(N * N, sizeof(double));
    if (!B_transpose) {
		return NULL;	
    }

    double *temp1 = calloc(N * N, sizeof(double));
    if (!temp1) {
		return NULL;	
    }

    double *temp2 = calloc(N * N, sizeof(double));
    if (!temp2) {
		return NULL;	
    }

    // calculez A_transpose si B_transpose
    transpose_upper(N, A, A_transpose);
    transpose_regular(N, B, B_transpose);

    // calculez A * B * A_transpose
    multiply_upper_with_regular(N, A, B, temp1);
    multiply_regular_with_lower(N, temp1, A_transpose, temp2);

    // calculez B_transpose * B_transpose
    multiply_regular_with_regular(N, B_transpose, B_transpose, temp1);

    // calculez result = A * B * A_transpose + B_transpose * B_transpose 
    add_two_matrices(N, temp2, temp1, result);

    free(temp1);
    free(temp2);
    free(A_transpose);
    free(B_transpose);
    return result;
}

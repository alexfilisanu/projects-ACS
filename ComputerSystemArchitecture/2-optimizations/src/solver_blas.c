/*
 * Tema 2 ASC
 * 2023 Spring
 */
#include "utils.h"
#include <cblas.h>
#include <string.h>

/* 
 * Add your BLAS implementation here
 */
double* my_solver(int N, double *A, double *B) {
	double *result = calloc(N * N, sizeof(double));
    if (!result) {
		return NULL;
	}

	// result = B
	cblas_dcopy(N * N, B, 1, result, 1);

	// inmultesc la stanga cu A, matrice superior triunghiulara => result = A * B
	cblas_dtrmm(CblasRowMajor, CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, N, N, 1.f, A, N, result, N);
    
	// inmultesc la dreapta cu A transpus => result = A * B * A^t
	cblas_dtrmm(CblasRowMajor, CblasRight, CblasUpper, CblasTrans, CblasNonUnit, N, N, 1.f, A, N, result, N);

	// adaug la rezultat B^t * B^t => result = A * B * A^t + B^t * B^t
	cblas_dgemm(CblasRowMajor, CblasTrans, CblasTrans, N, N, N, 1.f, B, N, B, N, 1.f, result, N);

	return result;
}

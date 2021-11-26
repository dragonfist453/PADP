#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// main function to do matrix multiplication using OpenMP
int main(int argc, char **argv) {
    int i, j, k;
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int p = atoi(argv[3]);
    int nthreads = atoi(argv[4]);
    int **A = (int **)malloc(n * sizeof(int *));
    int **B = (int **)malloc(m * sizeof(int *));
    int **C = (int **)malloc(n * sizeof(int *));
    for (i = 0; i < n; i++) {
        A[i] = (int *)malloc(m * sizeof(int));
        C[i] = (int *)malloc(p * sizeof(int));
    }
    for (i = 0; i < m; i++) {
        B[i] = (int *)malloc(p * sizeof(int));
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            A[i][j] = i + j;
        }
    }
    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            B[i][j] = i + j;
        }
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            C[i][j] = 0;
        }
    }
    omp_set_num_threads(nthreads);
    #pragma omp parallel for private(i, j, k)
    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            for (k = 0; k < m; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    return 0;
}
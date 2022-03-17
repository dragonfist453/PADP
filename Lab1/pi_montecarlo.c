#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define SEED 6897381

// Main function that calculates PI using montecarlo method
int main(int argc, char *argv[]) {
  int i, j, k, n, nthreads, ntrials;
  double x, y, pi, sum;
  double start, end;
  for(nthreads = 1; nthreads <= 8; nthreads*=2) {
    for(n=10000;n<=10000000;n*=10) {
        sum = 0.0;
        omp_set_num_threads(nthreads);
        start = omp_get_wtime();
        #pragma omp parallel for private(i, j, k, x, y) reduction(+:sum)
        for (i = 0; i < n; i++) {
            x = (double)rand() / RAND_MAX;
            y = (double)rand() / RAND_MAX;
            if (x * x + y * y <= 1) {
                sum++;
            }
        }
        pi = 4 * sum / n;
        end = omp_get_wtime();
        printf("%d Threads, %d Trials, pi = %f, Time = %lf\n", nthreads, n, pi, end-start);
    }
  }
  return 0;
}
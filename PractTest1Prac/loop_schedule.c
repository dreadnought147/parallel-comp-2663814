#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>

void heavy_compute(int *data, int n)
{
    for (int i = 0; i < n; ++i)
    {
        // work differs per i: simulate variable cost
        data[i] = (int)(sqrt(i) * log(i + 1));
    }
}

void heavy_compute_parl(int *data, int n)
{
#pragma omp parallel for schedule(auto)
    for (int i = 0; i < n; ++i)
    {
        // work differs per i: simulate variable cost
        data[i] = (int)(sqrt(i) * log(i + 1));
    }
}
int main()
{
    int n = 40000000;
    int *D = malloc(n * sizeof *D);
    double start, end;
    start = omp_get_wtime();
    heavy_compute(D, n);
    end = omp_get_wtime();
    printf("The serial runtime: %5f \n", end - start);

    //////Parllel run
    start = omp_get_wtime();
    heavy_compute_parl(D, n);
    end = omp_get_wtime();
    printf("The parallel runtime: %5f \n", end - start);

    printf("D[0]=%d, D[n-1]=%d\n", D[0], D[n - 1]);
    return 0;
}
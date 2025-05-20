#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

float sum_array(float *a, int n)
{

    float sum = 0.0f;
// Accumulate all values into sum
#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < n; ++i)
    {
        sum += a[i]; // ▶️ Hint: need thread-safe accumulation
    }

    return sum;
}
float sum_array_serial(float *a, int n)
{

    float sum = 0.0f;
    // Accumulate all values into sum
    // #pragma omp parallel for reduction(+ : sum)
    {
        for (int i = 0; i < n; ++i)
        {
            sum += a[i]; // ▶️ Hint: need thread-safe accumulation
        }
    }

    return sum;
}

int main()
{
    int n = 100000000;
    float *A = malloc(n * sizeof *A);
    for (int i = 0; i < n; ++i)
        A[i] = 1.0f;
    double end_time, start_time, total;
    //////parallel run
    start_time = omp_get_wtime();
    total = sum_array(A, n);
    end_time = omp_get_wtime();
    printf("time for parll run: %f \n", end_time - start_time);

    //////////serial run
    start_time = omp_get_wtime();
    total = sum_array_serial(A, n);
    end_time = omp_get_wtime();
    printf("timr fot serial run: %f \n", end_time - start_time);
    printf("Sum = %.1f\n", total);
    return 0;
}
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

void vec_add(float *a, float *b, float *c, long long n)
{
// Loop over elements: compute c[i] = a[i] + b[i]
#pragma omp parallel for
    for (int i = 0; i < n; ++i)
    {
        c[i] = a[i] + b[i]; // ▶️ Hint: each iteration independent
    }
}
void vec_add_serial(float *a, float *b, float *c, long long n)
{
    // Loop over elements: compute c[i] = a[i] + b[i]
    // #pragma omp parallel for
    for (int i = 0; i < n; ++i)
    {
        c[i] = a[i] + b[i]; // ▶️ Hint: each iteration independent
    }
}

int main()
{
    long long n = 100000000;
    float *A = malloc(n * sizeof *A);
    float *B = malloc(n * sizeof *B);
    float *C = malloc(n * sizeof *C);
    for (int i = 0; i < n; ++i)
    {
        A[i] = i * 1.0f;
        B[i] = (n - i) * 1.0f;
    }
    ////parllel run
    double end_time, start_time;
    start_time = omp_get_wtime();
    vec_add(A, B, C, n);
    end_time = omp_get_wtime();
    printf("timr fot parll run: %f \n", end_time - start_time);

    //////////serial run
    start_time = omp_get_wtime();
    vec_add_serial(A, B, C, n);
    end_time = omp_get_wtime();
    printf("timr fot serial run: %f \n", end_time - start_time);

    printf("C[0]=%.1f, C[n-1]=%.1f\n", C[0], C[n - 1]);

    free(A);
    free(B);
    free(C);
    return 0;
}
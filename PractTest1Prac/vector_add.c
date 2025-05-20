#include <stdio.h>
#include <stdlib.h>

void vec_add(float *a, float *b, float *c, int n)
{
    // Loop over elements: compute c[i] = a[i] + b[i]
    for (int i = 0; i < n; ++i)
    {
        c[i] = a[i] + b[i]; // ▶️ Hint: each iteration independent
    }
}

int main()
{
    int n = 1000000;
    float *A = malloc(n * sizeof *A);
    float *B = malloc(n * sizeof *B);
    float *C = malloc(n * sizeof *C);
    for (int i = 0; i < n; ++i)
    {
        A[i] = i * 1.0f;
        B[i] = (n - i) * 1.0f;
    }

    vec_add(A, B, C, n);
    printf("C[0]=%.1f, C[n-1]=%.1f\n", C[0], C[n - 1]);

    free(A);
    free(B);
    free(C);
    return 0;
}
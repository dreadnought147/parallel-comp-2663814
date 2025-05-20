#include <stdio.h>
#include <omp.h>

void compute_A()
{
    // simulate module A
    for (int i = 0; i < 10000000; ++i)
        ;
    printf("Done A\n");
}

void compute_B()
{
    // simulate module B
    for (int i = 0; i < 20000000; ++i)
        ;
    printf("Done B\n");
}

int main()
{
    // compute_A and compute_B do independent work
    double start, end;

    start = omp_get_wtime();
    compute_A();
    compute_B();
    compute_A();
    compute_B();
    end = omp_get_wtime();
    printf("The serial time is : %5f \n", end - start);

    start = omp_get_wtime();
#pragma omp parallel sections
    {
#pragma omp section
        compute_A(); // ▶️ Hint: could run concurrently
#pragma omp section
        compute_B();
#pragma omp section
        compute_A();
#pragma omp section
        compute_B();
    }
    end = omp_get_wtime();
    printf("The parll time is : %5f \n", end - start);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int fib_parl(int n)
{
    int x, y;
    if (n < 2)
        return n;
    if (n < 20)
        return fib_serial(n);

#pragma omp task shared(x)
    x = fib_parl(n - 1);

#pragma omp task shared(y)
    y = fib_parl(n - 2);

#pragma omp taskwait
    return x + y;
}

int fib_serial(int n)
{
    if (n < 2)
        return n;
    int x = fib_serial(n - 1);
    int y = fib_serial(n - 2);
    return x + y;
}

int main()
{
    double start, end;
    int result;
    int n = 45;
    start = omp_get_wtime();
    result = fib_serial(n); // ▶️ Hint: recursive tasks can split branches
    end = omp_get_wtime();
    printf("fib(%d) = %d\n", n, result);
    printf("The serial run is %5f  \n", end - start);

    start = omp_get_wtime();
#pragma omp parallel
    {
#pragma omp single
        result = fib_parl(n); // ▶️ Hint: recursive tasks can split branches
    }

    end = omp_get_wtime();
    printf("fib(%d) = %d\n", n, result);
    printf("The parallel run is %5f  \n", end - start);

    return 0;
}

// the parallel goes too deep for large n recurosvely so its imprtant to have a cutoff where we can switch to serial to avoid high overhead
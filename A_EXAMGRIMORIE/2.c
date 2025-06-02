// DATA SCOPING DIFF BETWEEN SHARED AND PRIV
#include <stdio.h>
#include <omp.h>

int main()
{
    int x = 5;

#pragma omp parallel private(x) // x is private (local copy in each thread)
    {
        x = omp_get_thread_num(); // Each thread modifies its own x
        printf("Thread %d has x = %d\n", omp_get_thread_num(), x);
    }

    printf("Main thread sees x = %d\n", x); // x is unchanged globally
    printf(" private ensures thread safety by giving each thread its own copy of x. \n ensures saftey from race condition,\n  whicg is which thread will get to update that vaiable first \n if it was shared(x) you will see that x's value will not remain 5 it will change and we are not sure what to, beacuse we \n cannot determine which thred will update it last/n");
    return 0;
}

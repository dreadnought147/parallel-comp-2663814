#include <stdio.h>
#include <omp.h>
// WE ARE TESTIUNG THE IF CLAUSE IN // CONTRUCT
int main()
{
    int n = 4;

#pragma omp parallel if (n > 5) // if false itll run this code like theres only one thread so itll print runnin... thread 0 beacuse 0 is master thread but nothing else, if its true itll run it in paralllel so therell be omp_get_num_thread outputs
    {
        printf("Running in parallel: thread %d\n", omp_get_thread_num());
    }

    return 0;
}

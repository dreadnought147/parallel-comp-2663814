#include <stdio.h>
#include <omp.h>
// SINGLE construct: only one thread executes the block, others wait or skip
int main()
{
#pragma omp parallel // team of 8 threads start working
    {
#pragma omp single // one designated thread performs initialization alone while others wait or proceed
        {
            printf("Thread %d initializes data\n", omp_get_thread_num());
        }
    }
    return 0;
}
// Concept: 'single' lets exactly one thread execute the code block, others do not execute it.
// Importance: ideal for setup or initialization before parallel processing begins.

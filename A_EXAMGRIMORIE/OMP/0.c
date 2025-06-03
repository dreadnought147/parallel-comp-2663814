#include <stdio.h>
#include <omp.h>

int main()
{
// Start a parallel region: each thread runs this block
#pragma omp parallel
    {
        printf("Hello from thread %d\n", omp_get_thread_num()); // omp get thread num fetches the ID of teh thread that runs the block of code,
                                                                // since all of tehm are, then its a matter of which thread ran fatser
    }
    printf("omp get thread num fetches the ID of teh thread that runs the block of code,// since all of tehm are, then its a matter of which thread ran fatser");
    return 0;
}
// the function is a way to show hoew to setup multiple threads hiting on program / blovk of code
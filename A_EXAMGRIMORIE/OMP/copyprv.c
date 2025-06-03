#include <stdio.h>
#include <omp.h>

int main()
{
    int val = 10;

    // Begin parallel region — multiple threads will be spawned here
#pragma omp parallel private(val)
    {
        // Only one thread (the "single" thread) executes the following block
        // Once it finishes, it broadcasts the value of 'val' to all other threads using copyprivate
#pragma omp single copyprivate(val)
        {
            scanf("%d", &val); // Only one thread reads input and sets 'val'
        }

        // All threads (including the one that read input) now use the copied value of 'val'
        printf("Thread %d sees val = %d\n", omp_get_thread_num(), val);
    }

    return 0;
}

#include <stdio.h>
#include <omp.h>
// SECTIONS construct: defines independent tasks executed by different threads
int main()
{
#pragma omp parallel sections // team of 8 threads, multiple independent tasks inside
    {
#pragma omp section // assign one thread to execute this task
        {
            printf("Thread %d does A\n", omp_get_thread_num());
        }

#pragma omp section // assign another thread to execute this task
        {
            printf("Thread %d does B\n", omp_get_thread_num());
        }

// With 8 threads but only 2 sections, 6 threads remain idle here if say the other 2 were still busy
#pragma omp section
        printf("Available threads: %d\n", omp_get_max_threads());
    }

    return 0;
}
// Concept: sections divides unrelated, independent tasks among threads.
// Unlike loop parallelism where threads do similar repeated work, sections let threads run different tasks that together solve a bigger problem.
// Importance: Useful for differnt-grained parallelism when tasks are distinct rather than repetitive.

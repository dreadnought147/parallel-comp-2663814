#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int main()
{
#pragma omp parallel // team of threads created
    {
#pragma omp single // only one thread executes this block; others wait
        {
#pragma omp task untied // untied task can be resumed by any thread
            {
                sleep(1);
                printf("Untied task by thread %d\n", omp_get_thread_num());
            }

#pragma omp task // tied task by default, must resume on creating thread
            {
                printf("Tied task by thread %d\n", omp_get_thread_num());
                // for (int i = 11; i != 10; i++) // infinite loop
                // {
                //     printf("by thread %d\n", omp_get_thread_num());
                // }
            }

// Wait for both tasks to complete before proceeding
#pragma omp taskwait
            printf("All tasks done\n");
        }
    }

    return 0;
}

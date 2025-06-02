#include <stdio.h>
#include <omp.h>

void work(int id)
{
    printf("Task %d by thread %d\n", id, omp_get_thread_num());
}

int main()
{
#pragma omp parallel
    {
#pragma omp single // one thread (e.g., thread 0) executes this block sequentially
        {
            for (int i = 0; i < 4; i++) // loop runs sequentially by single thread
            {
                // if true: create a task for this iteration, which other threads can execute in parallel
                // if false: the single thread executes the task immediately without creating a new task
#pragma omp task if (i < 2) final(i == 3)
                // For i=0,1: tasks are created and can run in parallel by idle threads (parallelism).
                // For i=2,3: 'if' is false, so the single thread executes these tasks immediately (no task creation overhead).
                // 'final(i == 3)' means if the task is for i=3, any nested tasks inside it will execute immediately (no further task creation). //  no more task generation, just run them immedialty
                work(i);
            }
        }
    }
    return 0;
}
// Summary:
// 'if' controls whether a task is deferred/created or executed immediately.
// 'final' controls whether nested tasks inside the current task are deferred or executed immediately.

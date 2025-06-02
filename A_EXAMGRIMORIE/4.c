#include <stdio.h>
#include <omp.h>
// CRITCIAL SECTIONS THESE ARE SYNCR CONCEPTS
int main()
{
    int counter = 0;

#pragma omp parallel shared(counter)
    {
#pragma omp critical // Only one thread in this section at a time but all threads will hit this block just in a line after the other so if tehres 8 threads we should get counter = 8
        {
            printf("Thread %d update counter = %d ", omp_get_thread_num(), counter);
            counter++;
            printf("to counter = %d \n", counter);
        }
    }

    printf("now if there was no critcical section, two threads may read the same value of counter and redundantly increemt to the same value because theyll be competeng to see who hits 'counter++' first \n");
    printf("Final counter = %d\n", counter);

    return 0;
}
// remarks
// see how we have counter as shared because we want "multple chefs working on the same dish" , but if we say private(counter) we are saying each chef should have thier dish to work on,
// work = increment so each thread will just update 0 to 1
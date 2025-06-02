#include <stdio.h>
#include <omp.h>
// FOR LOOP CONSTRUCTS
int main()
{
#pragma omp parallel for // for loops are extremly paralleizable
                         // why? => for loop is the same iteration or same logic done n amount of times so (0 , 1 , 2  ,3  , 4, 5,  6,  7,  8,   9 etc) now for construct is saying
                         // c decicate thread 0 the iterations (0-4) thread 1 (5-8) or whatever order this is an example baiscally its splitting the loop iterations evenyly among the threads
    for (int i = 0; i < 16; i++)
    {
        printf("Thread %d handles iteration %d\n", omp_get_thread_num(), i);
    }

    return 0;
    // now the inteested learner will ask what if the number of iterations % num of threads != 0 then we know theres remaineders,
    // this is the job of the OS, it will use load balancing to still try split it evenly but it isthe casde that some threads will handle way more load
}

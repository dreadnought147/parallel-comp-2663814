#include <stdio.h>
#include <omp.h>

int main()
{
#pragma omp parallel for collapse(2) // if you have  aperfectly or a canonical loop (search it) , you can used a collapse clause with a int passed in representing the number of of loops within the nesting ,  to collapse that loop into 1 for loop
                                     // if for (i up to n for (j up to n)) => collpase(2) => for (i up to n*n) then itll split tose iterations as it would norally for the for construct
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("Thread %d works on [%d][%d]\n", omp_get_thread_num(), i, j);
        }
    }

    return 0;
}
// 🔍 Concept: collapse(n) flattens n nested loops into one.
// 📌 Importance: Increases parallelism potential for multidimensional loops.
//
#include <stdio.h>
#include <omp.h>
// FOR LOOP CONSTRUCT
// CLAUD=SES ON SCHEDULINGS
// work division via the schedule clause
// assigns fixed size chunks to threads
// this helpn us know what our load balcing will look like
int main()
{
    omp_set_num_threads(8);
#pragma omp parallel for schedule(static, 2)
    for (int i = 0; i < 16; i++)
    {
        printf("Thread %d handles iteration %d\n", omp_get_thread_num(), i);
    }

    return 0;
}
// the interested learner will ask what will happen to remainders the allocations happen via round robin say we have 4 threads (0-3) and 9 iterations (0-8)
// 0  : 0 and 1 || then 0 : 8
// 1  : 2 and 3 || [done]
// 2  : 4 -5    ||
// 3  : 6 -7    ||
#include <stdio.h>
#include <omp.h>
// LAT PRIVATE CLAUSE
int main()
{
    int last = 0;

#pragma omp parallel for lastprivate(last)
    for (int i = 0; i < 10; i++) // the last assigned chunk say thread 3: 8 and 9 , last will be assined the last compuation of teh last iteration of the for loop
    // so last = i means last will be given the value 9 since its the last iterationof teh oor loop
    {
        last = i;
    }

    printf("Last value = %d\n", last);
    return 0;
}

#include <stdio.h>
#include <omp.h>

int main()
{
    omp_set_num_threads(4); // Request 4 threads (affects next parallel region)

#pragma omp parallel
    {
        // Only 4 threads max will run here
        printf("Thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
    }
    printf("point is to show that we can enforce how many threads we use, depending on the machine a certain amount would be too large \n");
    printf("notice how the thread that gets printed first is random this is becuase there is no sycnr, whichever finsihed the program first will output");

    return 0;
}

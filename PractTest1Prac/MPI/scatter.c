#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 16;
    int *array = NULL;
    int chunk = N / size;
    int *sub = malloc(chunk * sizeof *sub); // each sub array for each process will be chunk size wide

    if (rank == 0)
    {
        array = malloc(N * sizeof *array); // main array will 16 N size wide
        for (int i = 0; i < N; ++i)
        {
            array[i] = 1;
        }
    }
    ///////////////    // ▶️ distribute 'array' into 'sub' on each rank

    MPI_Scatter(array, chunk, MPI_INT,
                sub, chunk, MPI_INT,
                0, MPI_COMM_WORLD);
    int local_sum = 0;
    for (int i = 0; i < chunk; ++i)
        local_sum += sub[i];
    ///////////////////

    int *sums = NULL;
    if (rank == 0)
        sums = malloc(size * sizeof *sums); // where we gather
    // ▶️ gather each `local_sum` into `sums` on root
    MPI_Gather(&local_sum, 1, MPI_INT,
               sums, 1, MPI_INT,
               0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        int total = 0;
        for (int i = 0; i < size; ++i)
            total += sums[i];
        printf("Global sum = %d\n", total);
    }

    MPI_Finalize();
    return 0;
}

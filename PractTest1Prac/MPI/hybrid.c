#include <mpi.h>
#include <omp.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, NULL);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

// ▶️ inside this parallel region, distribute work across threads
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("Rank %d, thread %d says hi!\n", rank, tid);
        // Note: only master thread should call MPI functions under FUNNELED
    }

    MPI_Finalize();
    return 0;
}
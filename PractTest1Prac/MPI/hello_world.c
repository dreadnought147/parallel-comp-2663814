#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv); // ▶️ init
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Hello from rank %d of %d\n", rank, size);

    MPI_Finalize(); // ▶️ teardown
    return 0;
}

// what did we learn
// we compile a program using mpicc -fopenmp program.c -o output
// run a program using mpiexec run -n 4 ./output
// mpi upon initialization launches proccesors according to amount of cores available
// always initialises 0 first   etc
// but initializes can only finalize accroding to os scheduling
// printf is not synchronized
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int thot;
    if (rank == 0)
    {
        thot = 69; // wnat rank 0 to initialise data then ,
    }
    MPI_Bcast(&thot, 1, MPI_INT, 0, MPI_COMM_WORLD); // PI_Bcast is ran globally but only sender 0 can run it

    if (rank != 0)
    {
        printf("Process %d recieved a thot called %d from a pimp called %d \n", rank, thot, 0);
    }
    MPI_Finalize();
    return 0;
}
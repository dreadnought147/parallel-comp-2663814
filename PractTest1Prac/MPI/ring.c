#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //// uses rank and sizes for iterating processes to commincate with

    int next = (rank + 1) % size; // the next comm point will be immedialty after it sequatially, but just add % for wrap around
    int prev = (rank + size - 1) % size;
    int token = rank; // messgage we are sending is which proc communicated with us so if p0 => p1 the token recieved by p1 will be 0;
    /// performs it one one call to avoid dealocks
    MPI_Sendrecv(&token, 1, MPI_INT, next, 1,
                 &token, 1, MPI_INT, prev, 1,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Rank %d received token %d from %d\n", rank, token, prev);

    MPI_Finalize();
    return 0;
}
/// TAGS MUST MATCH FS
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int local = rank + 5; // each process has its own value
    int total;

    // ▶️ perform a sum reduction across all ranks into `total` at root
    MPI_Reduce(&local, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Sum of ranks+1 = %d\n", total);

    MPI_Finalize();
    return 0;
}
// thing abour reduce os that it doesnt specify a array to gather on its very siumilar to gather but it says from alllll processes
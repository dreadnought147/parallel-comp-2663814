// Question 7: Broadcast Communication
/*Testing: Collective communication patterns, specifically broadcast.
This tests understanding of one-to-many communication where one process distributes data to all others.*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int broadcast_data;

    if (rank == 0)
    {
        // Root process initializes the data to be broadcast
        broadcast_data = 12345;
        printf("Process 0 broadcasting value: %d\n", broadcast_data);
    }

    double start_time = MPI_Wtime();

    // MPI_Bcast sends data from root process to ALL processes in communicator
    // All processes (including root) must call this function
    // Parameters: data_ptr, count, datatype, root_rank, communicator
    MPI_Bcast(&broadcast_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    // After broadcast, all processes have the same value
    printf("Process %d received broadcast value: %d (time: %.6f s)\n",
           rank, broadcast_data, end_time - start_time);

    // Verify all processes have the same data
    int verification_sum;
    // Reduce operation to sum all values - should be broadcast_value * size
    MPI_Reduce(&broadcast_data, &verification_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); // REDUCE THE SUME IT SHOULD NBE == TO 1235*4
    // if (rank == 1)
    // {
    //     printf("Verification sum = %d : \n", verification_sum);
    // }
    if (rank == 0)
    {
        int expected_sum = broadcast_data * size;
        if (verification_sum == expected_sum)
        {
            printf("Broadcast verification: SUCCESS (sum = %d)\n", verification_sum);
        }
        else
        {
            printf("Broadcast verification: FAILED (expected %d, got %d)\n",
                   expected_sum, verification_sum);
        }
    }

    MPI_Finalize();
    return 0;
}
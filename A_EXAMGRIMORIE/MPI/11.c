// Question 11: All-to-All Communication
/*Testing: Complete data exchange patterns where every process sends data to every other process.
 This tests understanding of complex communication patterns.*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Each process sends one integer to every other process
    int *send_data = malloc(size * sizeof(int));
    int *recv_data = malloc(size * sizeof(int));

    // Prepare send data: process rank sends (rank*100 + dest) to destination dest
    printf("Process %d preparing send data:\n", rank);
    for (int i = 0; i < size; i++)
    {
        send_data[i] = (rank * 100) + i;
        printf("  to process %d: %d\n", i, send_data[i]);
    }

    // MPI_Alltoall: every process sends one element to every other process
    // Process i sends send_data[j] to process j
    // Process i receives into recv_data[j] from process j
    // Parameters: send_buf, send_count, send_type, recv_buf, recv_count, recv_type, comm
    MPI_Alltoall(send_data, 1, MPI_INT, recv_data, 1, MPI_INT, MPI_COMM_WORLD);
    // the primitivy i can give you here is that imagine each processhas a vector join these vectors to form a matrix
    // trans that matrix then seperate the columns to get new vectors ech vector is what sits on the recv of each process

    printf("Process %d received data:\n", rank);
    for (int i = 0; i < size; i++)
    {
        printf("  from process %d: %d\n", i, recv_data[i]);
    }

    // Verify correctness: recv_data[i] should equal i*100 + rank
    printf("Process %d verification:\n", rank);
    for (int i = 0; i < size; i++)
    {
        int expected = i * 100 + rank;
        if (recv_data[i] == expected)
        {
            printf("  from process %d: CORRECT (%d)\n", i, recv_data[i]);
        }
        else
        {
            printf("  from process %d: ERROR (got %d, expected %d)\n",
                   i, recv_data[i], expected);
        }
    }

    free(send_data);
    free(recv_data);
    MPI_Finalize();
    return 0;
}
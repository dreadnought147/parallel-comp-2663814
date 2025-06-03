// Question 9: Scatter and Gather Operations
/*Testing: Data distribution (scatter) and collection (gather) patterns.
 These are fundamental for distributing work across processes and collecting results*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *send_data = NULL;
    int *gathered_data = NULL;
    int local_data;

    if (rank == 0)
    {
        // Root process creates data array to scatter
        send_data = malloc(size * sizeof(int));
        gathered_data = malloc(size * sizeof(int));

        printf("Process 0 preparing data to scatter:\n");
        for (int i = 0; i < size; i++)
        {
            send_data[i] = (i + 1) * 10; // Values: 10, 20, 30,40...
            printf("  send_data[%d] = %d\n", i, send_data[i]);
        }
        // send data = [10,20,30,40]
    }
    //

    // MPI_Scatter distributes array elements: one element per process
    // Each process receives send_count elements starting from their rank*send_count
    // Parameters: send_buffer, send_count, send_type, recv_buffer, recv_count, recv_type, root, comm
    MPI_Scatter(send_data, 1, MPI_INT, &local_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received from scatter: %d\n", rank, local_data);

    // Each process does some computation on its local data
    local_data = local_data * rank + 100; // Transforms the data

    printf("Process %d computed result: %d\n", rank, local_data);

    // MPI_Gather collects one element from each process into an array at root
    // Parameters: send_buffer, send_count, send_type, recv_buffer, recv_count, recv_type, root, comm
    MPI_Gather(&local_data, 1, MPI_INT, gathered_data, 1, MPI_INT, 0, MPI_COMM_WORLD); // receive all those things but put it as an array

    if (rank == 0)
    {
        printf("Process 0 gathered results:\n");
        for (int i = 0; i < size; i++)
        {
            printf("  gathered_data[%d] = %d ", i, gathered_data[i]);
        }

        free(send_data);
        free(gathered_data);
    }

    MPI_Finalize();
    return 0;
}
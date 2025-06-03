#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    // Initialize MPI environment - must be called before any MPI functions
    // This sets up the communication infrastructure for all processes
    MPI_Init(&argc, &argv);

    int rank, size;

    // Get the rank (unique ID) of this process within the communicator
    // Rank determines which code path this process will execute
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get total number of processes in the communicator
    // Size helps determine workload distribution and communication patterns
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = 42;

    if (rank == 0)
    {
        // Process 0 acts as sender
        // MPI_Send: blocking send - doesn't return until message is safely sent
        // Parameters: data_ptr, count, datatype, destination_rank, tag, communicator
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent data: %d\n", data);
    }
    else if (rank == 1)
    {
        int received_data = 0;
        // MPI_Recv: blocking receive - waits until matching message arrives
        // Parameters: buffer_ptr, max_count, datatype, source_rank, tag, communicator, status
        // MPI_ANY_SOURCE and MPI_ANY_TAG can be used for wildcards
        printf("Process 1 received container has the value = : %d before the data was received \n", received_data);
        MPI_Recv(&received_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data: %d\n", received_data);
    }

    // Clean up MPI environment - must be called before program exits
    // Ensures all communication is completed and resources are freed
    MPI_Finalize();
    return 0;
}
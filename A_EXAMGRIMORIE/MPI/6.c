// Question 6: Timing with MPI_Wtime
/*Testing: Performance measurement using MPI_Wtime. Understanding timing is crucial
for benchmarking and optimizing MPI applications.*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data_size = 100000000; // 1 million integers
    int *data = malloc(data_size * sizeof(int));

    // Initialize data
    for (int i = 0; i < data_size; i++)
    {
        data[i] = rank * 1000 + i;
    }

    // Get high-resolution wall-clock time
    // MPI_Wtime returns time in seconds as a double
    double start_time = MPI_Wtime();

    if (rank == 0) // WE SEND
    {
        for (int dest = 1; dest < size; dest++)
        {
            // Time large message sends WE ARE SENDING VERY LARGE MESSEAGES
            MPI_Send(data, data_size, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }

        double end_time = MPI_Wtime();
        double elapsed = end_time - start_time;

        // Calculate bandwidth: (data_size * sizeof(int) * (size-1)) bytes / elapsed seconds
        double total_bytes = data_size * sizeof(int) * (size - 1); // sent how many bytes acorss the communcator in general
        double bandwidth_mbps = (total_bytes / elapsed) / (1024 * 1024);

        printf("Process 0: Sent %d integers to %d processes\n", data_size, size - 1);
        printf("Total time: %.6f seconds\n", elapsed);
        printf("Bandwidth: %.2f MB/s\n", bandwidth_mbps);
    }
    else
    {
        int *recv_buffer = malloc(data_size * sizeof(int));

        double recv_start = MPI_Wtime();
        MPI_Recv(recv_buffer, data_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        double recv_end = MPI_Wtime();

        printf("Process %d: Receive took %.6f seconds\n", rank, recv_end - recv_start);

        free(recv_buffer);
    }

    free(data);
    MPI_Finalize();
    return 0;
}
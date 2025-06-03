// Question 10: Variable-length Scatter and Gather (MPI_Scatterv/Gatherv)
/*Testing: Advanced collective operations for non-uniform data distribution.
This tests handling of variable-length messages, which is common in load-balancing scenarios*/
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
    int *send_counts = NULL; // How many elements to send to each process
    int *send_displs = NULL; // Starting position for each process's data
    int *recv_data = NULL;
    int recv_count, total_elements;

    if (rank == 0)
    {
        // Create arrays with different sizes for each process
        // Process i gets (i+1) elements
        send_counts = malloc(size * sizeof(int));
        send_displs = malloc(size * sizeof(int));

        total_elements = 0;
        for (int i = 0; i < size; i++)
        {
            send_counts[i] = i + 1;          // Process i gets (i+1) elements
            send_displs[i] = total_elements; // Starting position in send_data
            total_elements += send_counts[i];
        }

        send_data = malloc(total_elements * sizeof(int));

        // Fill data array
        printf("Process 0 preparing variable-length data:\n");
        for (int i = 0; i < total_elements; i++)
        {
            send_data[i] = i * 10;
            printf("  send_data[%d] = %d\n", i, send_data[i]);
        }

        printf("Send counts: ");
        for (int i = 0; i < size; i++)
        {
            printf("%d ", send_counts[i]);
        }
        printf("\n");
    }

    // Each process needs to know how much data it will receive
    MPI_Scatter(send_counts, 1, MPI_INT, &recv_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    recv_data = malloc(recv_count * sizeof(int));

    // MPI_Scatterv distributes variable amounts of data to each process
    // Parameters: send_buf, send_counts[], send_displs[], send_type,
    //            recv_buf, recv_count, recv_type, root, comm
    MPI_Scatterv(send_data, send_counts, send_displs, MPI_INT,
                 recv_data, recv_count, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received %d elements: ", rank, recv_count);
    for (int i = 0; i < recv_count; i++)
    {
        printf("%d ", recv_data[i]);
    }
    printf("\n");

    // Transform received data (multiply by rank)
    for (int i = 0; i < recv_count; i++)
    {
        recv_data[i] *= (rank + 1);
    }

    // Prepare for gathering variable amounts back to root
    int *recv_counts = NULL;
    int *recv_displs = NULL;
    int *final_data = NULL;

    if (rank == 0)
    {
        recv_counts = send_counts; // Reuse the same counts
        recv_displs = send_displs; // Reuse the same displacements
        final_data = malloc(total_elements * sizeof(int));
    }

    // MPI_Gatherv collects variable amounts of data from each process
    MPI_Gatherv(recv_data, recv_count, MPI_INT,
                final_data, recv_counts, recv_displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Process 0 gathered transformed data:\n");
        for (int i = 0; i < total_elements; i++)
        {
            printf("  final_data[%d] = %d\n", i, final_data[i]);
        }

        free(send_data);
        free(send_counts);
        free(send_displs);
        free(final_data);
    }

    free(recv_data);
    MPI_Finalize();
    return 0;
}
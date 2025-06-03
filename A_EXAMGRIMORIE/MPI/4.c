#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 4)
    {
        if (rank == 0)
            printf("This example needs at least 4 processes\n");
        MPI_Finalize();
        return 0;
    }
    /// we need 4 processes
    int send_data[3] = {(rank + 1) * 10, 1 * 20, 1 * 30};
    int recv_data[3];

    // Arrays to hold multiple request handles
    // Each non-blocking operation needs its own request
    MPI_Request requests[6]; // 3 sends + 3 receives
    MPI_Status statuses[6];

    if (rank == 0)
    {
        // Post multiple non-blocking sends to different processes
        // This creates a communication pattern where rank 0 fans out data
        for (int i = 0; i < 3; i++)
        {
            // diff in prev one is that we sent to one recv or one process now we are sending to multple process
            MPI_Isend(&send_data[i], 1, MPI_INT, i + 1, 100 + i, MPI_COMM_WORLD, &requests[i]); // req[0] req info for send to process 1 req[1] will info for send to pr2 and req[2] info for send at procc 3,
                                                                                                //
        }

        // Wait for ALL sends to complete before proceeding
        // count=3 because we have 3 send operations
        MPI_Waitall(3, requests, statuses); // START at req[0] and move 0 1 2

        printf("Process 0 completed all sends\n");
    }
    else if (rank >= 1 && rank <= 3)
    {
        // Each process posts receives from rank 0
        MPI_Irecv(&recv_data[rank - 1], 1, MPI_INT, 0, 100 + (rank - 1) /*mpi any tag*/,
                  MPI_COMM_WORLD, &requests[(rank + 2)]);

        // Wait for receive to complete
        MPI_Wait(&requests[(rank - 1) + 3], &statuses[(rank - 1) + 3]);

        printf("Process %d received: %d\n", rank, recv_data[rank - 1]);
    }

    MPI_Finalize();
    return 0;
}
// essentially we scttered data in pro 0 into the other processes, and also kept track of the status of the sneds because well it was nonblovking sends
// so esentially we just had checks to make sure that the data was sent to all 3
// and at each process we received the data coming to use safely
// then yeah
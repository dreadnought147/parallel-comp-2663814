#include <mpi.h>
#include <stdio.h>
// how do i query message informationusiing MPI_status
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        // rank 0 is our sender
        int data[3] = {10, 20, 30};
        // Send different amounts of data to different processes
        for (int i = 1; i < size; i++)
        {
            MPI_Send(data, i, MPI_INT, i, 100 + i, MPI_COMM_WORLD); // send to pro 1,2,3
        }
    }
    else
    {
        // pro 1,2,3

        int buffer[10];
        MPI_Status status;

        // Receive from any source with any tag
        // Status object will contain metadata about the received message
        // we odnt control the tag we sent to directly that much so we use wildcards for recv
        MPI_Recv(buffer, 10, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        // Extract source rank from status - who sent this message?
        int source = status.MPI_SOURCE;

        // Extract message tag from status - what type of message is this?
        int tag = status.MPI_TAG;

        // theres also int error = status.ERROR yes its an int and success = 0 im assuming anything ese is some differnt error

        int error = status.MPI_ERROR;
        printf("The error is : %d : ", error);

        // Get actual count of elements received using MPI_Get_count
        // This is essential when you don't know exact message size in advance
        int count;                               // container to stored vlaue of how many elemnts were received frpm most recewnt recv call
        MPI_Get_count(&status, MPI_INT, &count); // we send variable amount via the for loop then really check if we sent accordinly by using getcount

        printf("Process %d received %d elements from process %d with tag %d\n",
               rank, count, source, tag);
    }

    MPI_Finalize();
    return 0;
}
/*Question 3: Non-blocking Communication with MPI_Request
Testing: Asynchronous communication patterns using non-blocking send/receive operations. This enables overlap of computation and communication, critical for performance optimization.*/ \
#include<mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // the process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // ho amny

    int send_data = rank * 10;
    int recv_data;

    // MPI_Request objects track the status of non-blocking operations
    // Each non-blocking call needs its own request handle
    MPI_Request send_request, recv_request;

    if (rank == 0)
    {
        send_data = 100;
        // Non-blocking send: initiates send but returns immediately
        // Allows computation to continue while message is being transmitted
        MPI_Isend(&send_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &send_request); // the succes of that send should be stored in request

        // Simulate some computation that can happen while message is in transit
        for (int i = 0; i < 1000000; i++)
        {
            // Do some work... // instead of waiting for the send to be succesful and acked we continue doing some compuattion heavy stuff while the operation happens
            // blah blah blah // some othe rinitialization nonsense
        }

        // Wait for the send to complete before proceeding
        // This ensures the send buffer can be safely reused
        MPI_Wait(&send_request, MPI_STATUS_IGNORE);
    }
    else if (rank == 1)
    {
        // Non-blocking receive: posts receive but returns immediately
        // Process can do other work while waiting for message
        MPI_Irecv(&recv_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &recv_request); // we initaite a open to recv communication to tye communicator but we return immediatly to perform some computations

        // Do some computation while waiting for message
        int computation_result = 0;
        for (int i = 0; i < 1000000; i++)
        {
            computation_result += i;
        }
        // do those compute inten=sice stuff

        // Wait for receive to complete before using the data
        // This ensures the receive buffer contains the complete message
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);

        printf("Process 1 received: %d (computation result: %d)\n",
               recv_data, computation_result);
    }

    MPI_Finalize();
    return 0;
}

// here the logic is that we can send and recv without halting certain stuff that requite alot of compute because if we kknow send and recv gives latency
// then whiule we are waiting for that comm to occur we can do some local unrelated compuation to save time
/*Question 5: Testing Completion with MPI_Test*/
/*Testing: Non-blocking completion testing, which allows processes to check if operations are complete without blocking.
 This enables more flexible computation/communication overlap patterns.*/
#include <mpi.h>
#include <stdio.h>
#include <unistd.h> // for usleep

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = rank * 100;
    int recv_data;
    MPI_Request request;
    int flag = 0; // Will be set to 1 when operation completes

    if (rank == 0)
    {

        // Start non-blocking send
        MPI_Issend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

        int work_counter = 0;
        // Keep doing work until the send completes
        while (!flag)
        {
            // MPI_Test checks if operation is complete WITHOUT blocking
            // flag will be set to 1 if complete, 0 if still in progress
            MPI_Test(&request, &flag, MPI_STATUS_IGNORE); // while the data tries to send we have a sentinal that checks if the send is complete if its not complete then continou doing the work if oits complete
            // then our mpi test will set flag to true kicking us out the loop then

            // Do some work while waiting
            work_counter++;
            usleep(1000); // Simulate work (1ms)

            if (work_counter % 100 == 0)
            {
                printf("Process 0 still working... (iteration %d)\n", work_counter);
            }
        }

        printf("Process 0: Send completed after %d work iterations\n", work_counter);
    }
    else if (rank == 1)
    {
        flag = 0;
        // Add some delay to make the test more interesting
        usleep(500000); // 500ms delay

        // Start non-blocking receive
        MPI_Irecv(&recv_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

        int work_counter = 0;
        while (!flag)
        {
            // Test for completion without blocking
            MPI_Test(&request, &flag, MPI_STATUS_IGNORE);

            work_counter++;
            usleep(1000);

            if (work_counter % 100 == 0)
            {
                printf("Process 1 still waiting... (iteration %d)\n", work_counter);
            }
        }

        printf("Process 1 received: %d after %d iterations\n", recv_data, work_counter);
    }

    MPI_Finalize();
    return 0;
}
// POINT 2 POINT
#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, num_of_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);         // will assign a core this code and that core will be given a rank
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_procs); // will just get the amount of procceses we ave useful for math logic sto access otehrs
    // we like to think of the processors in some list or some array where they can comminucate to eat=ch othe rby just point to point communication but a square 2by2 array, where each block can talk to one with p2p
    int message;

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
    {

        message = 47;

        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); // WE SEND A MESSGE TO PROC 1, sends to communicator and waits for completion,
        printf("hELLO IM SAYING MESSAGE 47 \n");
        // /if sent then itll run on to next line which is recv in this case, Blocking send
        MPI_Recv(&message, 1, MPI_INT, 1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // AFTER SENDING WE ARE EXPECTING TO RECOVE SOMETHING BACK THATS USUALLY HOW COMMUNICATION WORKS
        printf("Rank 0 received message %d \n", message);
    }
    else if (rank == 1)
    {
        MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 1 got message  %d\n", message);
        message += 1;
        MPI_Send(&message, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
        printf("hELLO IM responidng to  MESSAGE 47 \n");
    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}

// okay cool what can we do to improve on rder if rank 0
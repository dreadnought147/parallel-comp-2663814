#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = rank;
    MPI_Request req_send, req_recv;
    MPI_Status status;

    int partner = (rank + 1) % size;
    int recv_buf;
    // ▶️ post non-blocking recv from partner
    MPI_Irecv(&recv_buf, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, &req_recv);
    // ▶️ post non-blocking send to partner
    MPI_Isend(&partner, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, &req_send);
    // return before communications complete, whats the requesst?

    // ▶️ do some local compute here
    // that we need to do regardles of communication successs.

    // ▶️ wait on both requests to complete
    // MPI_Wait(&req_recv, &status);
    // MPI_Wait(&req_send, MPI_STATUS_IGNORE);

    printf("Rank %d done non-blocking exchange.\n", rank);
    MPI_Finalize();
    return 0;
}
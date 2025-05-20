#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 8;               // matrix size NxN
    int rows_per = N / size; // rows per process
    double *A = NULL;
    double *local_A = malloc(rows_per * N * sizeof(double)); // 2*8
    double *x = malloc(N * sizeof(double));                  // 1*8 vector
    double *y_local = malloc(rows_per * sizeof(double));     // 1*2 scalar
    double *y = NULL;

    if (rank == 0)
    {
        A = malloc(N * N * sizeof(double));
        y = malloc(N * sizeof(double));

        for (int i = 0; i < N * N; ++i)
            A[i] = 1.0;
        for (int i = 0; i < N; ++i)
            x[i] = 1.0;
    }
    // ▶️ scatter rows of A to all ranks
    // ▶️ scatter rows of A to all ranks
    MPI_Scatter(A, rows_per * N, MPI_DOUBLE,
                local_A, rows_per * N, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    //; broadcast x to all
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // local multiply: for each local row i:
    for (int i = 0; i < rows_per; ++i)
    {
        y_local[i] = 0.0;
        for (int j = 0; j < N; ++j)
            y_local[i] += local_A[i * N + j] * x[j];
    }

    // ▶️ gather y_local into full result y on root
    MPI_Gather(y_local, rows_per, MPI_DOUBLE,
               y, rows_per, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Result y[0]=%.1f, y[N-1]=%.1f\n", /* first & last entry of y */ y[0], y[N - 1]);
    }

    MPI_Finalize();
    return 0;
}
/// here we have a 8 by 8  we split it accordign to its size to 4 proxess, each procer does the matrix multpication with a x thats bcasted to all processers
// at each pro we have a ylocal that computes the result of the mult at each pro, then that ylocal is gathered into a big y,
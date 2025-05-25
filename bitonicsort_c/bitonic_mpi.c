#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void merge(int *a, int *b, int *out, int size, int dir)
{
    int i = 0, j = 0, k = 0;
    if (dir)
    {
        while (i < size && j < size)
            out[k++] = (a[i] < b[j]) ? a[i++] : b[j++];
    }
    else
    {
        while (i < size && j < size)
            out[k++] = (a[i] > b[j]) ? a[i++] : b[j++];
    }
    while (i < size)
        out[k++] = a[i++];
    while (j < size)
        out[k++] = b[j++];
}

int cmp_inc(const void *a, const void *b) { return (*(int *)a - *(int *)b); }
int cmp_dec(const void *a, const void *b) { return (*(int *)b - *(int *)a); }

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size, N;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2)
    {
        if (rank == 0)
            printf("Usage: mpirun -np <p> ./bitonic_mpi <array_size>\n");
        MPI_Finalize();
        return 1;
    }

    N = atoi(argv[1]);
    if ((N & (N - 1)) != 0 || (size & (size - 1)) != 0 || N % size != 0)
    {
        if (rank == 0)
            printf("Array size and process count must be powers of 2, and divisible.\n");
        MPI_Finalize();
        return 1;
    }

    int local_N = N / size;
    int *data = NULL;
    int *local = malloc(local_N * sizeof(int));

    if (rank == 0)
    {
        data = malloc(N * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < N; i++)
            data[i] = rand() % 100000;
    }

    MPI_Scatter(data, local_N, MPI_INT, local, local_N, MPI_INT, 0, MPI_COMM_WORLD);
    qsort(local, local_N, sizeof(int), (rank % 2 == 0) ? cmp_inc : cmp_dec);

    double start = MPI_Wtime();

    for (int k = 2; k <= size; k <<= 1)
    {
        for (int j = k >> 1; j > 0; j >>= 1)
        {
            int partner = rank ^ j;
            int *recv = malloc(local_N * sizeof(int));
            MPI_Sendrecv(local, local_N, MPI_INT, partner, 0, recv, local_N, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            int up = ((rank & k) == 0);
            int keep = (rank < partner) == up;

            int *merged = malloc(2 * local_N * sizeof(int));
            merge(local, recv, merged, local_N, up);

            for (int i = 0; i < local_N; i++)
                local[i] = merged[(keep ? 0 : local_N) + i];

            free(merged);
            free(recv);
        }
    }

    double end = MPI_Wtime();
    if (rank == 0)
        printf("MPI Processes: %d | Time: %.6f sec\n", size, end - start);

    free(local);
    if (rank == 0)
        free(data);
    MPI_Finalize();
    return 0;
}

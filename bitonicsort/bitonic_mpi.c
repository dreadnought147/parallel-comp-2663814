#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int asc_cmp(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int desc_cmp(const void *a, const void *b)
{
    return (*(int *)b - *(int *)a);
}

void compare_and_exchange(int *local, int *partner, int count, int direction, int keep_min)
{
    int *temp = malloc(count * sizeof(int));
    for (int i = 0; i < count; i++)
    {
        if (keep_min)
            temp[i] = (local[i] < partner[i]) ? local[i] : partner[i];
        else
            temp[i] = (local[i] > partner[i]) ? local[i] : partner[i];
    }
    for (int i = 0; i < count; i++)
        local[i] = temp[i];
    free(temp);
}

void bitonic_sort_mpi(int *local, int count, int rank, int num_procs)
{
    for (int k = 2; k <= num_procs; k <<= 1)
    {
        for (int j = k >> 1; j > 0; j >>= 1)
        {
            int peer = rank ^ j;
            int direction = ((rank & k) == 0) ? 1 : 0;
            int *recv_buf = malloc(count * sizeof(int));
            MPI_Sendrecv(local, count, MPI_INT, peer, 0,
                         recv_buf, count, MPI_INT, peer, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if ((rank & k) == 0)
            {
                if ((rank & j) == 0)
                    compare_and_exchange(local, recv_buf, count, direction, 1);
                else
                    compare_and_exchange(local, recv_buf, count, direction, 0);
            }
            else
            {
                if ((rank & j) == 0)
                    compare_and_exchange(local, recv_buf, count, direction, 0);
                else
                    compare_and_exchange(local, recv_buf, count, direction, 1);
            }

            free(recv_buf);
        }
    }
}

int main(int argc, char **argv)
{
    int rank, num_procs;
    int total_n = (argc > 1) ? atoi(argv[1]) : (1 << 12);

    int *full_data = NULL, *sorted_data = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int chunk = total_n / num_procs;
    int *local = malloc(chunk * sizeof(int));
    int *quick_ref = malloc(chunk * sizeof(int));

    if (rank == 0)
    {
        full_data = malloc(total_n * sizeof(int));
        sorted_data = malloc(total_n * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < total_n; i++)
            full_data[i] = rand() % 10000;
    }

    MPI_Scatter(full_data, chunk, MPI_INT, local, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(full_data, chunk, MPI_INT, quick_ref, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank % 2 == 0)
        qsort(local, chunk, sizeof(int), asc_cmp);
    else
        qsort(local, chunk, sizeof(int), desc_cmp);

    double quick_start = MPI_Wtime();
    qsort(quick_ref, chunk, sizeof(int), asc_cmp);
    double quick_end = MPI_Wtime();

    double bitonic_start = MPI_Wtime();
    bitonic_sort_mpi(local, chunk, rank, num_procs);
    double bitonic_end = MPI_Wtime();

    MPI_Gather(local, chunk, MPI_INT, sorted_data, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(quick_ref, chunk, MPI_INT, full_data, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        int is_sorted = 1;
        for (int i = 1; i < num_procs; i++)
        {
            int prev_max = sorted_data[i * chunk - 1];
            int curr_min = sorted_data[i * chunk];
            if (prev_max > curr_min)
            {
                is_sorted = 0;
                break;
            }
        }
        printf("------------------------------------------------------------------\n");
        printf("MPI Bitonic Sort SORTED: %s\n", is_sorted ? "YES" : "NO");
        printf("QuickSort Time: %.6f s\n", quick_end - quick_start);
        printf("Bitonic Sort Time (MPI): %.6f s\n", bitonic_end - bitonic_start);
        printf("Speedup: %.6f\n", (quick_end - quick_start) / (bitonic_end - bitonic_start));
        // printf("array size: %d\n", total_n); used to check if mpi was sorting those dynamically sized arrays

        printf("------------------------------------------------------------------\n");
    }

    free(local);
    free(quick_ref);
    if (rank == 0)
    {
        free(full_data);
        free(sorted_data);
    }

    MPI_Finalize();
    return 0;
}

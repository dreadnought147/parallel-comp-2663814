#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void compareAndSwap(int *arr, int i, int j, int dir)
{
    if (dir == (arr[i] > arr[j]))
    {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

void bitonicMerge(int *arr, int low, int cnt, int dir)
{
    if (cnt > 1)
    {
        int k = cnt / 2;
#pragma omp parallel for
        for (int i = low; i < low + k; i++)
            compareAndSwap(arr, i, i + k, dir);
        bitonicMerge(arr, low, k, dir);
        bitonicMerge(arr, low + k, k, dir);
    }
}

void bitonicSort(int *arr, int low, int cnt, int dir)
{
    if (cnt > 1)
    {
        int k = cnt / 2;

        if (cnt < 2048)
        {
            bitonicSort(arr, low, k, 1);
            bitonicSort(arr, low + k, k, 0);
        }
        else
        {
#pragma omp task
            bitonicSort(arr, low, k, 1);
#pragma omp task
            bitonicSort(arr, low + k, k, 0);
#pragma omp taskwait
        }
        bitonicMerge(arr, low, cnt, dir);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./bitonic_omp <array_size>\n");
        return 1;
    }

    int N = atoi(argv[1]);
    if ((N & (N - 1)) != 0)
    {
        printf("Array size must be a power of 2.\n");
        return 1;
    }

    for (int threads = 2; threads <= 8; threads += 2)
    {
        int *arr = malloc(N * sizeof(int));
        for (int i = 0; i < N; i++)
            arr[i] = rand() % 100000;

        omp_set_num_threads(threads);
        double start = omp_get_wtime();
#pragma omp parallel
#pragma omp single
        bitonicSort(arr, 0, N, 1);
        double end = omp_get_wtime();

        printf("OMP Threads: %d | Time: %.6f sec\n", threads, end - start);
        free(arr);
    }

    return 0;
}

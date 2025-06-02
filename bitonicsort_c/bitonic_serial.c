#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void compareAndSwap(int *arr, int i, int j, int dir)
{
    if (dir == (arr[i] > arr[j]))
    {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void bitonicMerge(int *arr, int low, int cnt, int dir)
{
    if (cnt > 1)
    {
        int k = cnt / 2;
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
        bitonicSort(arr, low, k, 1);
        bitonicSort(arr, low + k, k, 0);
        bitonicMerge(arr, low, cnt, dir);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./bitonic <array_size>\n");
        return 1;
    }

    int N = atoi(argv[1]);
    if ((N & (N - 1)) != 0)
    {
        printf("Array size must be a power of 2.\n");
        return 1;
    }

    int *arr = malloc(N * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < N; i++)
        arr[i] = rand() % 100000;

    clock_t start = clock();
    bitonicSort(arr, 0, N, 1);
    clock_t end = clock();

    printf("Serial Time: %.6f sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(arr);
    return 0;
}

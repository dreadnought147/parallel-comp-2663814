#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <string.h>

void compare(int *arr, int i, int j, int direction)
{
    if (direction == (arr[i] > arr[j]))
    {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Example usage instead of bitonic_sort_serial:

void bitonic_merge_serial(int *arr, int low, int cnt, int direction)
{
    if (cnt > 1)
    {
        int k = cnt / 2;
        for (int i = low; i < low + k; i++)
        {
            compare(arr, i, i + k, direction);
        }
        bitonic_merge_serial(arr, low, k, direction);
        bitonic_merge_serial(arr, low + k, k, direction);
    }
}

void bitonic_sort_serial(int *arr, int low, int cnt, int direction)
{
    if (cnt > 1)
    {
        int k = cnt / 2;
        bitonic_sort_serial(arr, low, k, 1);
        bitonic_sort_serial(arr, low + k, k, 0);
        bitonic_merge_serial(arr, low, cnt, direction);
    }
}

void bitonic_merge_parallel(int *arr, int low, int cnt, int direction)
{
    if (cnt > 1)
    {
        int k = cnt / 2;
#pragma omp parallel for
        for (int i = low; i < low + k; i++)
        {
            compare(arr, i, i + k, direction);
        }
        bitonic_merge_parallel(arr, low, k, direction);
        bitonic_merge_parallel(arr, low + k, k, direction);
    }
}

void bitonic_sort_parallel(int *arr, int low, int cnt, int direction)
{
    if (cnt > 1)
    {
        int k = cnt / 2;
#pragma omp parallel sections
        {
#pragma omp section
            bitonic_sort_parallel(arr, low, k, 1);
#pragma omp section
            bitonic_sort_parallel(arr, low + k, k, 0);
        }
        bitonic_merge_parallel(arr, low, cnt, direction);
    }
}

int compare_ints(const void *a, const void *b)
{
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

int check(int *a, int *b, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <array_size>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if ((n & (n - 1)) != 0)
    {
        printf("Error: array size must be a power of 2\n");
        return 1;
    }

    int *arr = malloc(n * sizeof(int));
    int *arr_qs = malloc(n * sizeof(int));
    int *arr_serial = malloc(n * sizeof(int));
    int *arr_parallel = malloc(n * sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 10000;
    }

    memcpy(arr_qs, arr, n * sizeof(int));
    memcpy(arr_serial, arr, n * sizeof(int));
    memcpy(arr_parallel, arr, n * sizeof(int));

    double start_qs = omp_get_wtime();
    qsort(arr_qs, n, sizeof(int), compare_ints); // quicksort(arr_serial, 0, n - 1);
    double end_qs = omp_get_wtime();

    double start_serial = omp_get_wtime();
    bitonic_sort_serial(arr_serial, 0, n, 1);
    double end_serial = omp_get_wtime();

    int valid_serial = check(arr_serial, arr_qs, n);

    int threads[] = {2, 4, 8};
    printf("------------------------------------------------------------------\n");
    printf("Serial QUICKSORT sorted: %s\n", valid_serial ? "YES" : "NO");
    printf("Serial Time: %.6f s\n", end_qs - start_qs);

    // for (int i = 0; i < 3; i++)
    // {
    memcpy(arr_parallel, arr, n * sizeof(int));
    // omp_set_num_threads(threads[i]);
    double start_par = omp_get_wtime();
    bitonic_sort_parallel(arr_parallel, 0, n, 1);
    double end_par = omp_get_wtime();
    int valid = check(arr_parallel, arr_qs, n);
    double speedup = (end_qs - start_qs) / (end_par - start_par);

    printf("OpenMP Bitonic implem Sorted: %s  \n", valid ? "YES" : "NO");
    // printf("Threads: %d\n", threads[i]);
    printf("OpenMP Time: %.6f s\n", end_par - start_par);
    printf("Speedup: %.2f\n", speedup);
    printf("------------------------------------------------------------------\n");
    // }

    free(arr);
    free(arr_qs);
    free(arr_serial);
    free(arr_parallel);
    return 0;
}

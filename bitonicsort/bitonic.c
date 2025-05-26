#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void compare(int *arr, int i, int j, int dir)
{
    if (dir == (arr[i] > arr[j]))
    {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void bitonic_merge(int *arr, int low, int cnt, int dir)
{
    if (cnt > 1)
    {
        int k = cnt / 2;
        for (int i = low; i < low + k; i++)
        {
            compare(arr, i, i + k, dir);
        }
        bitonic_merge(arr, low, k, dir);
        bitonic_merge(arr, low + k, k, dir);
    }
}

void bitonic_sort(int *arr, int low, int cnt, int dir)
{
    if (cnt > 1)
    {
        int k = cnt / 2;
        bitonic_sort(arr, low, k, 1);
        bitonic_sort(arr, low + k, k, 0);
        bitonic_merge(arr, low, cnt, dir);
    }
}

int is_power_of_two(int n)
{
    return n > 0 && (n & (n - 1)) == 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <array_size>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (!is_power_of_two(n))
    {
        printf("Error: array size must be a power of 2.\n");
        return 1;
    }

    // Static test array
    int test[16] = {3, 7, 4, 8, 6, 2, 1, 5, 9, 0, 11, 13, 12, 15, 14, 10};
    printf("Static Test Array (Unsorted):\n");
    for (int i = 0; i < 16; i++)
        printf("%d ", test[i]);
    printf("\n");

    bitonic_sort(test, 0, 16, 1);

    printf("Static Test Array (Sorted):\n");
    for (int i = 0; i < 16; i++)
        printf("%d ", test[i]);
    printf("\n");

    // Dynamic large array for performance test
    int *arr = malloc(n * sizeof(int));
    if (!arr)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 100000;
    }

    clock_t start = clock();
    bitonic_sort(arr, 0, n, 1);
    clock_t end = clock();

    // Optional check
    // for (int i = 1; i < n; i++)
    //     if (arr[i - 1] > arr[i]) { printf("Sort failed.\n"); break; }

    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Bitonic Sort (Serial) on %d elements took %.6f seconds.\n", n, time_taken);

    free(arr);
    return 0;
}

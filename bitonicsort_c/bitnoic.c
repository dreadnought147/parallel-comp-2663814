#include <stdio.h>
#include <stdlib.h>

void compare_and_swap(int *arr, int i, int j, int dir)
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
            compare_and_swap(arr, i, i + k, dir);
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

int main()
{
    int n = 16;
    int arr[16] = {3, 7, 4, 8, 6, 2, 1, 5, 15, 14, 13, 11, 12, 10, 9, 0};

    bitonic_sort(arr, 0, n, 1);

    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
    return 0;
}

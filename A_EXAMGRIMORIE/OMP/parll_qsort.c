// Parallel quicksort using OpenMP tasks
// Tests use of task construct, task creation control, and understanding of fork-join in recursive context
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include <omp.h>
#include <stdio.h>

void quicksort(int *arr, int low, int high, int depth)
{
    if (low < high)
    {
        int pivot = arr[high]; // select pivot
        int i = low - 1;

        for (int j = low; j < high; j++)
        {
            if (arr[j] < pivot)
            {
                i++;
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }

        i++;
        int tmp = arr[i];
        arr[i] = arr[high];
        arr[high] = tmp;

        int max_depth = 8; // threshold depth to avoid overspawning tasks

        // Parallel region starts once to prevent redundant spawning of threads
        if (depth < max_depth)
        {
#pragma omp task shared(arr) firstprivate(low, i, depth) // low and i change at every call so we want the thread to capture the correct values
            quicksort(arr, low, i - 1, depth + 1);       // task created for left subarray

#pragma omp task shared(arr) firstprivate(high, i, depth) // array is shared make sense
            quicksort(arr, i + 1, high, depth + 1);       // task created for right subarray

#pragma omp taskwait // ensure tasks finish before moving forward
        }
        else
        {
            // if max depth reached, fallback to sequential recursion to avoid overhead
            quicksort(arr, low, i - 1, depth + 1);
            quicksort(arr, i + 1, high, depth + 1);
        }
    }
}
void quicksort_ser(int *arr, int low, int high)
{
    if (low < high)
    {
        int pivot = arr[high]; // select pivot
        int i = low - 1;

        for (int j = low; j < high; j++)
        {
            if (arr[j] < pivot)
            {
                i++;
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }

        i++;
        int tmp = arr[i];
        arr[i] = arr[high];
        arr[high] = tmp;

        quicksort_ser(arr, low, i - 1);
        quicksort_ser(arr, i + 1, high);
    }
}
int main()
{
    int n;
    scanf("%d", &n);
    int *arr = malloc(n * sizeof(int));
    // int arr = sizeof(arr) / sizeof(arr[0]);
    if (arr == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < n; i++)
    {
        arr[i] = n - i;
        // printf("%d ", arr[i]);
    }

    // Create a single parallel region with one master thread spawning tasks
    double start = omp_get_wtime();
#pragma omp parallel // parallel driver
    {
#pragma omp single // only one thread initiates the quicksort
        {
            quicksort(arr, 0, n - 1, 0);
        }
    }
    double parlltime = omp_get_wtime() - start;
    printf("parl time took is : %.5f \n ", parlltime);
    double sstart = omp_get_wtime();
    quicksort_ser(arr, 0, n - 1);
    double time = omp_get_wtime() - sstart;
    printf("serial time is : %.5f \n", time);
    // for (int i = 0; i < n; i++)
    //     printf("%d ", arr[i]);
    // printf("\n");
    free(arr);
    return 0;
}

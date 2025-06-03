#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ASCENDING 1
#define DESCENDING 0

// Swaps elements at indices i and j if they are out of order based on the direction
void compare_and_swap(int *a, int i, int j, int dir)
{
    // If sorting in ascending order and a[i] > a[j], or descending and a[i] < a[j], swap them
    if ((dir == ASCENDING && a[i] > a[j]) || (dir == DESCENDING && a[i] < a[j]))
    {
        int tmp = a[i]; // temporary variable to hold value
        a[i] = a[j];    // assign a[j] to a[i]
        a[j] = tmp;     // complete the swap
    }
}

// Merges a bitonic sequence starting at index 'low' of length 'count' in given direction
void bitonic_merge(int *a, int low, int count, int dir)
{
    if (count > 1) // only proceed if there are more than 1 element to merge
    {
        int k = count / 2; // divide the segment in half

        // Compare elements in the first half with the corresponding in the second half
        // Parallelize the loop using OpenMP: each iteration of compare_and_swap is independent
#pragma omp parallel for
        for (int i = low; i < low + k; i++)
        {
            compare_and_swap(a, i, i + k, dir); // perform compare-and-swap on pair
        }

        // Recursively merge the two halves (serial inside parallel tasks)
        bitonic_merge(a, low, k, dir);     // merge the first half
        bitonic_merge(a, low + k, k, dir); // merge the second half
    }
}

// Recursively sorts a sequence into a bitonic sequence and then merges it
void bitonic_sort(int *a, int low, int count, int dir)
{
    if (count > 1) // proceed only if the segment has more than one element
    {
        int k = count / 2; // divide the segment in half

        // First half sorted in ascending order to create bitonic sequence
        // Use OpenMP task to allow this recursive call to run concurrently with the next one
#pragma omp task shared(a) // auto passed in as firstprivate so
        bitonic_sort(a, low, k, ASCENDING);

        // Second half sorted in descending order
#pragma omp task shared(a)
        bitonic_sort(a, low + k, k, DESCENDING);

        // Wait until both above recursive calls finish before merging
#pragma omp taskwait

        // Merge the entire sequence in the specified direction
        bitonic_merge(a, low, count, dir);
    }
}

// Entry point for parallel bitonic sort
void parallel_bitonic_sort(int *a, int N)
{
    // Create a parallel region shared by all threads
#pragma omp parallel
    {
        // Only a single thread initiates the sort to prevent redundant work
        // But this thread will create tasks that are executed by all threads
#pragma omp single
        {
            bitonic_sort(a, 0, N, ASCENDING); // start bitonic sort on full array in ascending order
        }
    }
}

// Utility function to print the array
void print_array(int *a, int N)
{
    for (int i = 0; i < N; i++)
        printf("%d ", a[i]);
    printf("\n");
}

int main()
{
    int N = 16; // Size of the array (must be a power of 2 for bitonic sort)

    // Sample unsorted input array
    int a[16] = {10, 30, 11, 20, 4, 33, 6, 8, 9, 2, 1, 15, 14, 7, 5, 3};

    printf("Original array:\n");
    print_array(a, N); // print the unsorted array

    parallel_bitonic_sort(a, N); // sort the array using parallel bitonic sort

    printf("Sorted array:\n");
    print_array(a, N); // print the sorted array

    return 0;
}

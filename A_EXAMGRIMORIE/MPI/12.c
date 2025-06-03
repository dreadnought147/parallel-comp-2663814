// Question 12: Prefix Sum with MPI_Scan
/*Testing: Parallel prefix operations (scan/reduce-scatter patterns).
This tests understanding of algorithms that build results incrementally across processes.*/
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Each process contributes its rank + 1 to the prefix sum
    int local_value = rank + 1;
    int prefix_sum;

    printf("Process %d contributing: %d\n", rank, local_value);

    // MPI_Scan computes inclusive prefix reduction
    // Process i receives the reduction of values from processes 0 through i
    // For sum operation: process i gets sum of (local_values[0] + ... + local_values[i])
    MPI_Scan(&local_value, &prefix_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("Process %d prefix sum: %d\n", rank, prefix_sum);

    // Verify result: prefix sum at rank r should be 1+2+...+(r+1) = (r+1)*(r+2)/2
    int expected = (rank + 1) * (rank + 2) / 2;
    if (prefix_sum == expected)
    {
        printf("Process %d: PREFIX SUM CORRECT\n", rank);
    }
    else
    {
        printf("Process %d: PREFIX SUM ERROR (got %d, expected %d)\n",
               rank, prefix_sum, expected);
    }

    // Use prefix sum to compute exclusive prefix (often more useful)
    // Exclusive prefix at rank r is the sum of elements before rank r
    int exclusive_prefix = prefix_sum - local_value;
    printf("Process %d exclusive prefix: %d\n", rank, exclusive_prefix);

    // Practical application: use exclusive prefix for array indexing
    // If each process has local_value elements, exclusive_prefix gives starting index
    printf("Process %d: if I have %d elements, my starting index would be %d\n",
           rank, local_value, exclusive_prefix);

    MPI_Finalize();
    return 0;
}
// Question 8: Reduction Operations
/*Testing: Collective reduction operations that combine data from all processes. This tests understanding of many-to-one communication with mathematical operations*/
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Each process contributes its rank to various reduction operations
    int local_value = rank + 1; // Values: 1, 2, 3, ..., size

    int sum_result, max_result, min_result;

    printf("Process %d contributing value: %d\n", rank, local_value);

    // MPI_Reduce combines values from all processes using specified operation
    // Result is stored only at the root process (rank 0)

    // Sum all values: should be 1+2+3+...+size = size*(size+1)/2
    MPI_Reduce(&local_value, &sum_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Find maximum value: should be size
    MPI_Reduce(&local_value, &max_result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    // Find minimum value: should be 1
    MPI_Reduce(&local_value, &min_result, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0)
    { // BECUZ ALL THOSE CALCED VALUES ARE IN PROCESS 0'S MEMORY
        int expected_sum = size * (size + 1) / 2;
        printf("Reduction results:\n");
        printf("  Sum: %d (expected: %d)\n", sum_result, expected_sum);
        printf("  Max: %d (expected: %d)\n", max_result, size);
        printf("  Min: %d (expected: 1)\n", min_result);
    }

    // MPI_Allreduce performs reduction AND distributes result to ALL processes REDUCES THEN BCASTS
    // Every process gets the final result
    int global_sum;
    MPI_Allreduce(&local_value, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("Process %d: Global sum via Allreduce = %d\n", rank, global_sum); // PRINT 4 TIMES BECAUSE NOW ALL PROCESS HAVE THE VALUE

    MPI_Finalize();
    return 0;
}
# MPI Practice Exam - 20 Progressive Questions

## Question 1: Basic MPI Setup and Point-to-Point Communication

**Testing:** Understanding MPI initialization, basic send/receive operations, and proper MPI program structure. This tests your ability to establish communication between processes - the foundation of all MPI programming.

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize MPI environment - must be called before any MPI functions
    // This sets up the communication infrastructure for all processes
    _______(______, ______);

    int rank, size;

    // Get the rank (unique ID) of this process within the communicator
    // Rank determines which code path this process will execute
    MPI_Comm_rank(_______, &rank);

    // Get total number of processes in the communicator
    // Size helps determine workload distribution and communication patterns
    _______(MPI_COMM_WORLD, &size);

    int data = 42;

    if (rank == 0) {
        // Process 0 acts as sender
        // MPI_Send: blocking send - doesn't return until message is safely sent
        // Parameters: data_ptr, count, datatype, destination_rank, tag, communicator
        _______(______, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent data: %d\n", data);
    }
    else if (rank == 1) {
        int received_data;
        // MPI_Recv: blocking receive - waits until matching message arrives
        // Parameters: buffer_ptr, max_count, datatype, source_rank, tag, communicator, status
        // MPI_ANY_SOURCE and MPI_ANY_TAG can be used for wildcards
        MPI_Recv(&received_data, ______, ______, ______, ______, ______, MPI_STATUS_IGNORE);
        printf("Process 1 received data: %d\n", received_data);
    }

    // Clean up MPI environment - must be called before program exits
    // Ensures all communication is completed and resources are freed
    _______();
    return 0;
}
```

## Question 2: MPI Status Object and Message Metadata

**Testing:** Understanding how to query message information using MPI_Status. This is crucial for writing robust MPI programs that can handle variable-length messages and determine message sources dynamically.

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        int data[3] = {10, 20, 30};
        // Send different amounts of data to different processes
        for (int i = 1; i < size; i++) {
            MPI_Send(data, i, MPI_INT, i, 100 + i, MPI_COMM_WORLD);
        }
    }
    else {
        int buffer[10];
        MPI_Status status;

        // Receive from any source with any tag
        // Status object will contain metadata about the received message
        MPI_Recv(buffer, 10, MPI_INT, _______, _______, MPI_COMM_WORLD, &_______);

        // Extract source rank from status - who sent this message?
        int source = _______.MPI_SOURCE;

        // Extract message tag from status - what type of message is this?
        int tag = _______._______;

        // Get actual count of elements received using MPI_Get_count
        // This is essential when you don't know exact message size in advance
        int count;
        _______(_______, MPI_INT, &count);

        printf("Process %d received %d elements from process %d with tag %d\n",
               rank, count, source, tag);
    }

    MPI_Finalize();
    return 0;
}
```

## Question 3: Non-blocking Communication with MPI_Request

**Testing:** Asynchronous communication patterns using non-blocking send/receive operations. This enables overlap of computation and communication, critical for performance optimization.

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data = rank * 10;
    int recv_data;

    // MPI_Request objects track the status of non-blocking operations
    // Each non-blocking call needs its own request handle
    MPI_Request send_request, recv_request;

    if (rank == 0) {
        // Non-blocking send: initiates send but returns immediately
        // Allows computation to continue while message is being transmitted
        _______(_______, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &_______);

        // Simulate some computation that can happen while message is in transit
        for (int i = 0; i < 1000000; i++) {
            // Do some work...
        }

        // Wait for the send to complete before proceeding
        // This ensures the send buffer can be safely reused
        _______(_______, MPI_STATUS_IGNORE);

    }
    else if (rank == 1) {
        // Non-blocking receive: posts receive but returns immediately
        // Process can do other work while waiting for message
        MPI_Irecv(_______, _______, _______, _______, _______, _______, &_______);

        // Do some computation while waiting for message
        int computation_result = 0;
        for (int i = 0; i < 1000000; i++) {
            computation_result += i;
        }

        // Wait for receive to complete before using the data
        // This ensures the receive buffer contains the complete message
        MPI_Wait(_______, _______);

        printf("Process 1 received: %d (computation result: %d)\n",
               recv_data, computation_result);
    }

    MPI_Finalize();
    return 0;
}
```

## Question 4: Multiple Non-blocking Operations with MPI_Waitall

**Testing:** Managing multiple simultaneous non-blocking operations. This pattern is essential for complex communication patterns where a process needs to communicate with multiple partners simultaneously.

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 4) {
        if (rank == 0) printf("This example needs at least 4 processes\n");
        MPI_Finalize();
        return 0;
    }

    int send_data[3] = {rank * 10, rank * 20, rank * 30};
    int recv_data[3];

    // Arrays to hold multiple request handles
    // Each non-blocking operation needs its own request
    MPI_Request requests[6];  // 3 sends + 3 receives
    MPI_Status statuses[6];

    if (rank == 0) {
        // Post multiple non-blocking sends to different processes
        // This creates a communication pattern where rank 0 fans out data
        for (int i = 0; i < 3; i++) {
            _______(_______, 1, MPI_INT, i + 1, 100 + i, MPI_COMM_WORLD, &requests[i]);
        }

        // Wait for ALL sends to complete before proceeding
        // count=3 because we have 3 send operations
        _______(3, _______, _______);

        printf("Process 0 completed all sends\n");

    }
    else if (rank >= 1 && rank <= 3) {
        // Each process posts receives from rank 0
        MPI_Irecv(&recv_data[rank-1], 1, MPI_INT, 0, 100 + (rank-1),
                  MPI_COMM_WORLD, &requests[0]);

        // Wait for receive to complete
        MPI_Wait(_______, _______);

        printf("Process %d received: %d\n", rank, recv_data[rank-1]);
    }

    MPI_Finalize();
    return 0;
}
```

## Question 5: Testing Completion with MPI_Test

**Testing:** Non-blocking completion testing, which allows processes to check if operations are complete without blocking. This enables more flexible computation/communication overlap patterns.

```c
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>  // for usleep

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = rank * 100;
    int recv_data;
    MPI_Request request;
    int flag = 0;  // Will be set to 1 when operation completes

    if (rank == 0) {
        // Start non-blocking send
        MPI_Isend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

        int work_counter = 0;
        // Keep doing work until the send completes
        while (!flag) {
            // MPI_Test checks if operation is complete WITHOUT blocking
            // flag will be set to 1 if complete, 0 if still in progress
            _______(_______, &_______, MPI_STATUS_IGNORE);

            // Do some work while waiting
            work_counter++;
            usleep(1000);  // Simulate work (1ms)

            if (work_counter % 100 == 0) {
                printf("Process 0 still working... (iteration %d)\n", work_counter);
            }
        }

        printf("Process 0: Send completed after %d work iterations\n", work_counter);

    }
    else if (rank == 1) {
        // Add some delay to make the test more interesting
        usleep(500000);  // 500ms delay

        // Start non-blocking receive
        _______(_______, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &_______);

        int work_counter = 0;
        while (!_______) {
            // Test for completion without blocking
            MPI_Test(_______, _______, _______);

            work_counter++;
            usleep(1000);

            if (work_counter % 100 == 0) {
                printf("Process 1 still waiting... (iteration %d)\n", work_counter);
            }
        }

        printf("Process 1 received: %d after %d iterations\n", recv_data, work_counter);
    }

    MPI_Finalize();
    return 0;
}
```

## Question 6: Timing with MPI_Wtime

**Testing:** Performance measurement using MPI_Wtime. Understanding timing is crucial for benchmarking and optimizing MPI applications.

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data_size = 1000000;  // 1 million integers
    int *data = malloc(data_size * sizeof(int));

    // Initialize data
    for (int i = 0; i < data_size; i++) {
        data[i] = rank * 1000 + i;
    }

    // Get high-resolution wall-clock time
    // MPI_Wtime returns time in seconds as a double
    double start_time = _______();

    if (rank == 0) {
        for (int dest = 1; dest < size; dest++) {
            // Time large message sends
            MPI_Send(data, data_size, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }

        double end_time = MPI_Wtime();
        double elapsed = _______ - _______;

        // Calculate bandwidth: (data_size * sizeof(int) * (size-1)) bytes / elapsed seconds
        double total_bytes = data_size * sizeof(int) * (size - 1);
        double bandwidth_mbps = (total_bytes / elapsed) / (1024 * 1024);

        printf("Process 0: Sent %d integers to %d processes\n", data_size, size-1);
        printf("Total time: %.6f seconds\n", elapsed);
        printf("Bandwidth: %.2f MB/s\n", bandwidth_mbps);

    }
    else {
        int *recv_buffer = malloc(data_size * sizeof(int));

        double recv_start = MPI_Wtime();
        MPI_Recv(recv_buffer, _______, _______, _______, _______, _______, MPI_STATUS_IGNORE);
        double recv_end = _______();

        printf("Process %d: Receive took %.6f seconds\n", rank, recv_end - recv_start);

        free(recv_buffer);
    }

    free(data);
    MPI_Finalize();
    return 0;
}
```

## Question 7: Broadcast Communication

**Testing:** Collective communication patterns, specifically broadcast. This tests understanding of one-to-many communication where one process distributes data to all others.

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int broadcast_data;

    if (rank == 0) {
        // Root process initializes the data to be broadcast
        broadcast_data = 12345;
        printf("Process 0 broadcasting value: %d\n", broadcast_data);
    }

    double start_time = MPI_Wtime();

    // MPI_Bcast sends data from root process to ALL processes in communicator
    // All processes (including root) must call this function
    // Parameters: data_ptr, count, datatype, root_rank, communicator
    _______(_______, 1, MPI_INT, _______, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    // After broadcast, all processes have the same value
    printf("Process %d received broadcast value: %d (time: %.6f s)\n",
           rank, broadcast_data, end_time - start_time);

    // Verify all processes have the same data
    int verification_sum;
    // Reduce operation to sum all values - should be broadcast_value * size
    _______(_______, &verification_sum, 1, MPI_INT, _______, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int expected_sum = broadcast_data * size;
        if (verification_sum == expected_sum) {
            printf("Broadcast verification: SUCCESS (sum = %d)\n", verification_sum);
        } else {
            printf("Broadcast verification: FAILED (expected %d, got %d)\n",
                   expected_sum, verification_sum);
        }
    }

    MPI_Finalize();
    return 0;
}
```

## Question 8: Reduction Operations

**Testing:** Collective reduction operations that combine data from all processes. This tests understanding of many-to-one communication with mathematical operations.

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Each process contributes its rank to various reduction operations
    int local_value = rank + 1;  // Values: 1, 2, 3, ..., size

    int sum_result, max_result, min_result;

    printf("Process %d contributing value: %d\n", rank, local_value);

    // MPI_Reduce combines values from all processes using specified operation
    // Result is stored only at the root process (rank 0)

    // Sum all values: should be 1+2+3+...+size = size*(size+1)/2
    _______(_______, &sum_result, 1, MPI_INT, _______, 0, MPI_COMM_WORLD);

    // Find maximum value: should be size
    MPI_Reduce(_______, _______, _______, _______, MPI_MAX, _______, _______);

    // Find minimum value: should be 1
    MPI_Reduce(_______, _______, _______, _______, _______, _______, _______);

    if (rank == 0) {
        int expected_sum = size * (size + 1) / 2;
        printf("Reduction results:\n");
        printf("  Sum: %d (expected: %d)\n", sum_result, expected_sum);
        printf("  Max: %d (expected: %d)\n", max_result, size);
        printf("  Min: %d (expected: 1)\n", min_result);
    }

    // MPI_Allreduce performs reduction AND distributes result to ALL processes
    // Every process gets the final result
    int global_sum;
    _______(_______, &global_sum, 1, MPI_INT, MPI_SUM, _______);

    printf("Process %d: Global sum via Allreduce = %d\n", rank, global_sum);

    MPI_Finalize();
    return 0;
}
```

## Question 9: Scatter and Gather Operations

**Testing:** Data distribution (scatter) and collection (gather) patterns. These are fundamental for distributing work across processes and collecting results.

```c
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *send_data = NULL;
    int *gathered_data = NULL;
    int local_data;

    if (rank == 0) {
        // Root process creates data array to scatter
        send_data = malloc(size * sizeof(int));
        gathered_data = malloc(size * sizeof(int));

        printf("Process 0 preparing data to scatter:\n");
        for (int i = 0; i < size; i++) {
            send_data[i] = (i + 1) * 10;  // Values: 10, 20, 30, ...
            printf("  send_data[%d] = %d\n", i, send_data[i]);
        }
    }

    // MPI_Scatter distributes array elements: one element per process
    // Each process receives send_count elements starting from their rank*send_count
    // Parameters: send_buffer, send_count, send_type, recv_buffer, recv_count, recv_type, root, comm
    _______(send_data, 1, MPI_INT, &local_data, _______, _______, _______, MPI_COMM_WORLD);

    printf("Process %d received from scatter: %d\n", rank, local_data);

    // Each process does some computation on its local data
    local_data = local_data * rank + 100;  // Transform the data

    printf("Process %d computed result: %d\n", rank, local_data);

    // MPI_Gather collects one element from each process into an array at root
    // Parameters: send_buffer, send_count, send_type, recv_buffer, recv_count, recv_type, root, comm
    _______(_______, 1, MPI_INT, gathered_data, _______, _______, _______, _______);

    if (rank == 0) {
        printf("Process 0 gathered results:\n");
        for (int i = 0; i < size; i++) {
            printf("  gathered_data[%d] = %d\n", i, gathered_data[i]);
        }

        free(send_data);
        free(gathered_data);
    }

    MPI_Finalize();
    return 0;
}
```

## Question 10: Variable-length Scatter and Gather (MPI_Scatterv/Gatherv)

**Testing:** Advanced collective operations for non-uniform data distribution. This tests handling of variable-length messages, which is common in load-balancing scenarios.

```c
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *send_data = NULL;
    int *send_counts = NULL;    // How many elements to send to each process
    int *send_displs = NULL;    // Starting position for each process's data
    int *recv_data = NULL;
    int recv_count;

    if (rank == 0) {
        // Create arrays with different sizes for each process
        // Process i gets (i+1) elements
        send_counts = malloc(size * sizeof(int));
        send_displs = malloc(size * sizeof(int));

        int total_elements = 0;
        for (int i = 0; i < size; i++) {
            send_counts[i] = i + 1;  // Process i gets (i+1) elements
            send_displs[i] = total_elements;  // Starting position in send_data
            total_elements += send_counts[i];
        }

        send_data = malloc(total_elements * sizeof(int));

        // Fill data array
        printf("Process 0 preparing variable-length data:\n");
        for (int i = 0; i < total_elements; i++) {
            send_data[i] = i * 10;
            printf("  send_data[%d] = %d\n", i, send_data[i]);
        }

        printf("Send counts: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", send_counts[i]);
        }
        printf("\n");
    }

    // Each process needs to know how much data it will receive
    _______(send_counts, 1, MPI_INT, &recv_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    recv_data = malloc(recv_count * sizeof(int));

    // MPI_Scatterv distributes variable amounts of data to each process
    // Parameters: send_buf, send_counts[], send_displs[], send_type,
    //            recv_buf, recv_count, recv_type, root, comm
    _______(send_data, _______, _______, MPI_INT,
            recv_data, _______, MPI_INT, _______, MPI_COMM_WORLD);

    printf("Process %d received %d elements: ", rank, recv_count);
    for (int i = 0; i < recv_count; i++) {
        printf("%d ", recv_data[i]);
    }
    printf("\n");

    // Transform received data (multiply by rank)
    for (int i = 0; i < recv_count; i++) {
        recv_data[i] *= (rank + 1);
    }

    // Prepare for gathering variable amounts back to root
    int *recv_counts = NULL;
    int *recv_displs = NULL;
    int *final_data = NULL;

    if (rank == 0) {
        recv_counts = send_counts;  // Reuse the same counts
        recv_displs = send_displs;  // Reuse the same displacements
        final_data = malloc(total_elements * sizeof(int));
    }

    // MPI_Gatherv collects variable amounts of data from each process
    _______(_______, recv_count, MPI_INT,
            final_data, _______, _______, MPI_INT, _______, _______);

    if (rank == 0) {
        printf("Process 0 gathered transformed data:\n");
        for (int i = 0; i < total_elements; i++) {
            printf("  final_data[%d] = %d\n", i, final_data[i]);
        }

        free(send_data);
        free(send_counts);
        free(send_displs);
        free(final_data);
    }

    free(recv_data);
    MPI_Finalize();
    return 0;
}
```

## Question 11: All-to-All Communication

**Testing:** Complete data exchange patterns where every process sends data to every other process. This tests understanding of complex communication patterns.

```c
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Each process sends one integer to every other process
    int *send_data = malloc(size * sizeof(int));
    int *recv_data = malloc(size * sizeof(int));

    // Prepare send data: process rank sends (rank*100 + dest) to destination dest
    printf("Process %d preparing send data:\n", rank);
    for (int i = 0; i < size; i++) {
        send_data[i] = rank * 100 + i;
        printf("  to process %d: %d\n", i, send_data[i]);
    }

    // MPI_Alltoall: every process sends one element to every other process
    // Process i sends send_data[j] to process j
    // Process i receives into recv_data[j] from process j
    // Parameters: send_buf, send_count, send_type, recv_buf, recv_count, recv_type, comm
    _______(send_data, 1, MPI_INT, recv_data, _______, _______, _______);

    printf("Process %d received data:\n", rank);
    for (int i = 0; i < size; i++) {
        printf("  from process %d: %d\n", i, recv_data[i]);
    }

    // Verify correctness: recv_data[i] should equal i*100 + rank
    printf("Process %d verification:\n", rank);
    for (int i = 0; i < size; i++) {
        int expected = i * 100 + rank;
        if (recv_data[i] == expected) {
            printf("  from process %d: CORRECT (%d)\n", i, recv_data[i]);
        } else {
            printf("  from process %d: ERROR (got %d, expected %d)\n",
                   i, recv_data[i], expected);
        }
    }

    free(send_data);
    free(recv_data);
    MPI_Finalize();
    return 0;
}
```

## Question 12: Prefix Sum with MPI_Scan

**Testing:** Parallel prefix operations (scan/reduce-scatter patterns). This tests understanding of algorithms that build results incrementally across processes.

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
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
    _______(_______, &prefix_sum, 1, MPI_INT, _______, MPI_COMM_WORLD);

    printf("Process %d prefix sum: %d\n", rank, prefix_sum);

    // Verify result: prefix sum at rank r should be 1+2+...+(r+1) = (r+1)*(r+2)/2
    int expected = (rank + 1) * (rank + 2) / 2;
    if (prefix_sum == expected) {
        printf("Process %d: PREFIX SUM CORRECT\n", rank);
    } else {
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
```

## Question 13: Contiguous Derived Datatype

**Testing:** Creating and using derived datatypes for efficient communication of regular data structures. This tests understanding of how to optimize communication for structured data.

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
```

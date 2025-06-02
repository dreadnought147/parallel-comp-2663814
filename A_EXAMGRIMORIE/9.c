#include <stdio.h>
#include <omp.h>
// REDUCTION CLAUSE
int main()
{
    int total = 0;

#pragma omp parallel for reduction(+ : total)
    for (int i = 1; i <= 100; i++)
    {
        total += i;
    }

    printf("Total sum = %d\n", total);
    return 0;
}
// ITS IMPORTANT TO HAVE A MENTAL PICTURE OF WHAT THIS LOOKS LIKE
// imagine you have a big narrow long box with 8 sections so something like [ 1 | 2  | 3  | 4  | 5 | 6 |  7 | 8 ]
// for construct will say each thread say there are 4 threads will get to work on 2 subboxes so [1|2] and [3|4] etc for each thread 0 1 2 3
// lets take thread 0 its progarms says total = toatal + i thefore itll will have a private copy of total = 3 after running total+=i for [1|2]
// each thread will tehre fore have their own copy of total which will have their own values
// reductionn baisally says take all those private copies and aggregate them according to "+" which was the passed in operation baiscally just summing them all across the threads to have final value of total

/// better explanation/
// Think of the loop as a long narrow box split into 8 compartments: [1 | 2 | 3 | 4 | 5 | 6 | 7 | 8]
// If we have 4 threads, and use static scheduling with chunk size 2,
// each thread gets 2 compartments: e.g., Thread 0 gets [1 | 2], Thread 1 gets [3 | 4], etc.
// Now, suppose each thread runs: total += i;
// Each thread will have its *own private copy* of 'total', e.g., Thread 0 gets total = 3 (1+2)
// So we end up with 4 private totals (one per thread).
// The `reduction(+:total)` clause tells OpenMP to *combine all these private totals*
// using the '+' operator to compute the final shared total.
// Final result = sum of all thread-local totals.

// THIS IS EQUIVALENT TO REDUCTIONS
int main2()
{
    int total = 0;
    int i;

#pragma omp parallel // A team of threads (like 8 warriors) enters the battlefield
    {
        int private_total = 0; // Each thread has its own private loot bag, starting at 0

#pragma omp for // The battle is divided: each warrior is assigned a set of enemies (iterations)
        for (i = 1; i <= 100; i++)
        {
            private_total += i; // Each warrior collects loot from their assigned battles (local sum)
        }

// All threads now have their own collected loot
#pragma omp critical // Only one warrior at a time may approach the shared treasury
        {
            total += private_total; // Safely add personal loot to the shared total
        }
    }

    printf("Total sum = %d\n", total);
    return 0;
}
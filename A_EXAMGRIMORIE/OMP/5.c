#include <stdio.h>
#include <omp.h>
// ATOMIC SECTIONS // SYNC STUFF
int main()
{
    int sum = 0;

#pragma omp parallel for // we will explain this for shortly
    for (int i = 0; i < 100; i++)
    {
#pragma omp atomic // doe sthe same thing crictical does except its faster, so crictal expects a whole block of one thread some functionalty
                   // wehreas atomic expects simple lines suc as counter++ or sum++ invloving simpel operations
        sum += 1;  // Faster than critical, safe for simple ops such as ++ * -- etc
    }

    printf("Final sum = %d\n", sum);
    return 0;
}
// 🔍 Concept: atomic is lightweight, works for simple operations.
// 📌 Importance: Improves performance over critical.
#include <stdio.h>
#include <omp.h>
//  BARRIER, SYNC STUFF AGAIN
// NOTE THAT sync is just a way to control the ranomness of thread managemnt or threads in general, threads are wild horses that run rampant, we use sync constructs to guide them in stuff like one horse enter a stable at a time (crictcial)
// horses runnon the same lane or each have their own dedicated lanes , stupid analogy i know (privating certain varaibles so they)
int main()
{
#pragma omp parallel
    {
        printf("Before barrier: Thread %d\n", omp_get_thread_num());

#pragma omp barrier // All threads wait here
        // sleep(5);

        printf("After barrier: Thread %d\n", omp_get_thread_num());
        // without barrier if a hread ran this code it say thread 7 we would print before .. thread 7 then immedialtlt after it would be after bariier... thread 7
        // the barrier construct says after running this code wait here (barrier construct (until all other threads get here aswell)) then continue running the program
    }

    return 0;
}
// 🔍 Concept: barrier ensures all threads reach the same point.
// 📌 Importance: Essential for ordered execution stages.

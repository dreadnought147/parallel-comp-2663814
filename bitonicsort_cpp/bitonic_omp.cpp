#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <numeric>
#include <algorithm> // For std::swap and std::equal

using namespace std;

const int THRESHOLD = 2048;   // Granularity control
const int NUM_ITERATIONS = 5; // Number of times to run each test for averaging

void parallel_bitonicmerge(vector<int> &arry, int low, int count, int dir)
{
    if (count > 1)
    {
        int k = count / 2;
#pragma omp parallel for if (count > THRESHOLD) shared(arry, low, k, dir)
        for (int i = low; i < low + k; i++)
        {
            if (dir == (arry[i] > arry[i + k]))
            {
                swap(arry[i], arry[i + k]);
            }
        }

        if (count > THRESHOLD)
        {
#pragma omp task shared(arry)
            parallel_bitonicmerge(arry, low, k, dir);
#pragma omp task shared(arry)
            parallel_bitonicmerge(arry, low + k, k, dir);
#pragma omp taskwait
        }
        else
        {
            parallel_bitonicmerge(arry, low, k, dir);
            parallel_bitonicmerge(arry, low + k, k, dir);
        }
    }
}

void omp_bitonicsort(vector<int> &arry, int low, int count, int dir)
{
    if (count > 1)
    {
        int k = count / 2;
        if (count > THRESHOLD)
        {
#pragma omp task shared(arry)
            omp_bitonicsort(arry, low, k, 1);
#pragma omp task shared(arry)
            omp_bitonicsort(arry, low + k, k, 0);
#pragma omp taskwait
        }
        else
        {
            omp_bitonicsort(arry, low, k, 1);
            omp_bitonicsort(arry, low + k, k, 0);
        }
        parallel_bitonicmerge(arry, low, count, dir);
    }
}

void bitonicmerge(vector<int> &arry, int low, int count, int dir)
{
    if (count > 1)
    {
        int k = count / 2;
        for (int i = low; i < low + k; i++)
        {
            if (dir == (arry[i] > arry[i + k]))
            {
                swap(arry[i], arry[i + k]);
            }
        }
        bitonicmerge(arry, low, k, dir);
        bitonicmerge(arry, low + k, k, dir);
    }
}

void serial_bitonicsort(vector<int> &arry, int low, int count, int dir)
{
    if (count > 1)
    {
        int k = count / 2;
        serial_bitonicsort(arry, low, k, 1);
        serial_bitonicsort(arry, low + k, k, 0);
        bitonicmerge(arry, low, count, dir);
    }
}

vector<int> generateRandomArray(int power)
{
    int size = pow(2, power);
    vector<int> arr(size);
    for (int i = 0; i < size; ++i)
    {
        arr[i] = rand() % 1000;
    }
    return arr;
}

int main()
{
    int n;
    cout << "Enter power (n) for array size 2^n: ";
    cin >> n;
    srand(time(nullptr)); // Seed with current time
    int array_size = pow(2, n);

    cout << "Generated array of size: " << array_size << endl;

    // --- Serial Sort Performance ---
    vector<double> serial_times(NUM_ITERATIONS);
    vector<int> serial_result;
    for (int iter = 0; iter < NUM_ITERATIONS; ++iter)
    {
        vector<int> arr_serial = generateRandomArray(n);
        double start_serial = omp_get_wtime();
        serial_bitonicsort(arr_serial, 0, arr_serial.size(), 1);
        double end_serial = omp_get_wtime();
        serial_times[iter] = end_serial - start_serial;
        if (iter == 0)
            serial_result = arr_serial; // Store result for comparison
    }
    double avg_serial_time = accumulate(serial_times.begin(), serial_times.end(), 0.0) / NUM_ITERATIONS;
    cout << "Average serial bitonic sort time: " << avg_serial_time << " seconds\n";

    // --- OpenMP Sort Performance ---
    cout << "\n--- OpenMP Performance ---" << endl;
    for (int num_threads = 1; num_threads <= 32; num_threads *= 2)
    {
        vector<double> omp_times(NUM_ITERATIONS);
        for (int iter = 0; iter < NUM_ITERATIONS; ++iter)
        {
            vector<int> arr_omp = generateRandomArray(n);
            omp_set_num_threads(num_threads);
            double start_omp = omp_get_wtime();
#pragma omp parallel
#pragma omp single
            omp_bitonicsort(arr_omp, 0, arr_omp.size(), 1);
            double end_omp = omp_get_wtime();
            omp_times[iter] = end_omp - start_omp;

            // Optional: Correctness check for the first iteration
            if (iter == 0 && array_size <= 256)
            {
                bool correct = equal(arr_omp.begin(), arr_omp.end(), serial_result.begin());
                cout << "Threads " << num_threads << ": OpenMP sort " << (correct ? "correct" : "incorrect") << endl;
            }
        }
        double avg_omp_time = accumulate(omp_times.begin(), omp_times.end(), 0.0) / NUM_ITERATIONS;
        double speedup = avg_serial_time / avg_omp_time;
        cout << "======================" << endl;
        cout << "Threads: " << num_threads << endl;
        cout << "Average OpenMP bitonic sort time: " << avg_omp_time << " seconds" << endl;
        cout << "Speedup: " << speedup << endl;
    }

    return 0;
}
// Dot product using loop unrolling and static scheduling
// schedule(static, chunk_size) helps control iteration chunking per thread

float a[1000], b[1000];
float dot = 0.0;

#pragma omp parallel for schedule(static, 10) reduction(+ : dot)
for (int i = 0; i < 1000; i += 2)
{
    dot += a[i] * b[i];
    dot += a[i + 1] * b[i + 1]; // unrolling two iterations manually
}
///???
// Mandelbrot set pixel count with collapse and reduction
// Collapse collapses 2D loops into one, reduction handles safe summation

int compute_mandelbrot(double x0, double y0, int max_iter);

int main()
{
    int width = 800, height = 800;
    int max_iter = 1000;
    int total = 0;

#pragma omp parallel for collapse(2) reduction(+ : total)
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            double x0 = (i - width / 2.0) * 4.0 / width;
            double y0 = (j - height / 2.0) * 4.0 / height;
            int value = compute_mandelbrot(x0, y0, max_iter);
            total += value;
        }
    }

    printf("Total intensity: %d\n", total);
}
// essewntailly all the madelbrot does is iterate through a 2d array compute certain values using those iterates, then those vlues sare passes in the mandelbrot function and its parllwlization comes from simplky collapsing the nestedx loop
void kernel_omp(unsigned char *ptr, int i)
{
#pragma omp parallel num_threads(i)
    {
        // printf("i am thread number %d \n", omp_get_thread_num());
        int id = omp_get_thread_num();
        int tthreads = omp_get_num_threads();
        int rows_per_thread = DIM / tthreads;

        int start_row = id * rows_per_thread;
        int end_row = (id == tthreads - 1) ? DIM : (id + 1) * rows_per_thread;

        for (int y = 0; y < DIM; y++)
        {
            for (int x = start_row; x < end_row; x++)
            {
                int offset = x + y * DIM;
                int juliaValue = julia(x, y);
                ptr[offset * 4 + 0] = 255 * juliaValue;
                ptr[offset * 4 + 1] = 0;
                ptr[offset * 4 + 2] = 0;
                ptr[offset * 4 + 3] = 255;
            }
        }
    }
}
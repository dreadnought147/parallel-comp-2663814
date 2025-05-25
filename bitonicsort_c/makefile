CC=mpicc
CFLAGS=-fopenmp -O2

all: bitonic_serial bitonic_omp bitonic_mpi

bitonic: bitonic.c
	gcc $(CFLAGS) $< -o $@

bitonic_omp: bitonic_omp.c
	gcc $(CFLAGS) $< -o $@

bitonic_mpi: bitonic_mpi.c
	$(CC) $< -o $@

clean:
	rm -f bitonic bitonic_omp bitonic_mpi


CC = mpicc
CFLAGS = -fopenmp -O2

all: serial omp mpi

serial:
	$(CC) bitonic.c -o bitonic

omp:
	$(CC) bitonic_omp.c -o bitonic_omp $(CFLAGS)

mpi:
	$(CC) bitonic_mpi.c -o bitonic_mpi

clean:
	rm -f bitonic bitonic_omp bitonic_mpi

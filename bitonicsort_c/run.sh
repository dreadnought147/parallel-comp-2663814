#!/bin/bash

echo "Running Serial:"
./bitonic_serial 256

echo -e "\nRunning OpenMP:"
for t in 2 4 6 8; do
    OMP_NUM_THREADS=$t ./bitonic_omp 32768
done

echo -e "\nRunning MPI:"
for p in 2 4 ; do
    mpirun -np $p ./bitonic_mpi 32768
done

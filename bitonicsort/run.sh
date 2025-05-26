#!/bin/bash

# Compile everything fresh
make clean
make all

# Input sizes
Ns=(4096 8192 16384)
Threads=(2 4)
Processes=(2 4)

for N in "${Ns[@]}"; do
  echo ""
  echo "===========ARRAY SIZE $N============="
   echo "===================================="
  echo "---- Serial (QuickSort) ----"
  ./bitonic $N

  echo -e "\\n=====OpenMP Tests===== ----"
  for T in "${Threads[@]}"; do
    echo "THREADS:  $T"
    export OMP_NUM_THREADS=$T
    ./bitonic_omp $N
  done

  echo -e "\\n======MPI Tests========"
  for P in "${Processes[@]}"; do
    echo "MPI SLOTS:  $P "
    mpiexec -np $P ./bitonic_mpi $N
  done

  echo -e "\\n"
done

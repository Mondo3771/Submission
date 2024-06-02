#!/bin/bash
# ./run.sh <number_of_elements> <number_of_threads> <number_of_runs>
echo "We will be runnig each algorithm $3 times with 2^$1 elements "

echo "--------------------------------------------------------------------------------------------------------"
echo "Running bitonic_sort..."
./bitonic $1 $3
echo "--------------------------------------------------------------------------------------------------------"

echo "Running bitonic_sort_openmp..."
./bitonic_openmp $1 $2 $3
echo "--------------------------------------------------------------------------------------------------------"
 
echo "Running bitonic_sort_mpi..."
mpiexec -n 4 ./bitonic_mpi $1 $3
echo "--------------------------------------------------------------------------------------------------------"
 

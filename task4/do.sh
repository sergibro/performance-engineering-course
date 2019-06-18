#!/bin/bash
make clean
make
mpirun -np 4 ./mpi_min
mpirun -np 4 ./mpi_sum

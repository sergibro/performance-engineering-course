all: mpi_min mpi_sum

mpi_min: mpi_min.cpp
	mpicc -O3 -g3 -march=skylake -o mpi_min ./mpi_min.cpp

mpi_sum: mpi_sum.cpp
	mpicc -O3 -g3 -march=skylake -o mpi_sum ./mpi_sum.cpp

clean:
	rm mpi_min mpi_sum
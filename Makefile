all: solve_mpi, solve_openmp, solve_pthread

solve_mpi: 3way-mpi
	cd 3way-mpi && make mpi

solve_openmp:
	cd 3way-openmp && make omp

solve_pthread:
	cd 3way-pthread && make pthread
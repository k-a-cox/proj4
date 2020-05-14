CC=mpicc

all: mpi, clean

mpi: mpi.c
	$(CC) -o mpi.out mpi.c
	mpirun mpi.out

clean: mpi.out
	# rm *.out
	rm mpi



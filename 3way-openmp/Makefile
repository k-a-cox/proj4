CC=gcc

all: omp

omp: omp.c
	$(CC) -fopenmp -o omp omp.c
	./omp

clean:
	rm omp_1_thread

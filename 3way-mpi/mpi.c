#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define NUM_THREADS 1
int NUM_THREADS;

#define NUMBER_LINES 1000000
#define MAX_LINE_SIZE 2005 //It seems the max line size is 2001, but that is only a guess

// Contains the output
char char_array[NUMBER_LINES][MAX_LINE_SIZE];
int line_sum[NUMBER_LINES];
int local_line_sum[NUMBER_LINES];

void init_arrays(){
	int i;
	
	for (i = 0; i < NUMBER_LINES - 1; i++){
		line_sum[i] = 0;
	}
	
	//Read in the main file into char_array
	FILE* file = fopen("/homes/dan/625/wiki_dump.txt", "r");
	// FILE* file = fopen("test_file.txt", "r");
	for(i = 0; i < NUMBER_LINES; i++) {
		if (file == NULL) {
			printf("File not Found\n");
			exit(-1);
		}
		fgets(char_array[i], MAX_LINE_SIZE, file);
		
		//printf("%d: %s", i, char_array[i]);
	}
	fclose(file);
	printf("\n");
}

void count_chunk(void *rank){
	char theChar;
	int i, j, sum, startLine, endLine;
	int id = *((int *) rank);
	
	sum = 0;
	startLine = id * (NUMBER_LINES / NUM_THREADS);
	endLine = startLine + (NUMBER_LINES / NUM_THREADS);
	if (id == NUM_THREADS - 1) endLine = NUMBER_LINES;
	
	// Ready for parallelization

	printf("ID: %d\n", id);
	printf("Start line: %d\n", startLine);
	printf("End line: %d\n", endLine - 1);

	for(i = startLine; i < endLine; i++) {
		sum = 0;
		for(j = 0; j < MAX_LINE_SIZE; j++){
			theChar = char_array[i][j];
			if(theChar == '\0'){
				// printf("Length: %d\n",j); //Used to test how long to make each str
				break;
			}
			sum += theChar;
		}
		local_line_sum[i] = sum;
        
	}
	
}

void print_results(){
	int i;
	
	for(i = 0; i < NUMBER_LINES - 1; i++){
		printf("%d-%d: %d\n",i,i+1,line_sum[i]-line_sum[i+1]); //The computation moves here for simplicity of merging. 
                                                     //It could instead go into count but we would have to worry about boundry cases of the array
        fflush(stdout);
	}
}

main(int argc, char *argv[]){
    
    int i, rc;
    int numtasks, rank;
    MPI_Status Status;

    rc = MPI_Init(&argc,&argv);
    if (rc != MPI_SUCCESS) {
        printf ("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    NUM_THREADS = numtasks;
    printf("size = %d rank = %d\n", numtasks, rank);
    fflush(stdout);

    if ( rank == 0 ) {
            init_arrays();
    }
    MPI_Bcast(char_array, NUMBER_LINES * MAX_LINE_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);

    count_chunk(&rank);

    MPI_Reduce(local_line_sum, line_sum, NUMBER_LINES, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


    if ( rank == 0 ) {
            print_results();
    }

    MPI_Finalize();
    return 0;
}

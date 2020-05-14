#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 1

#define NUMBER_LINES 1000000
#define MAX_LINE_SIZE 2005 //It seems the max line size is 2001, but that is only a guess

// Contains the output
char char_array[NUMBER_LINES][MAX_LINE_SIZE];
int line_sum[NUMBER_LINES - 1];

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

void count_chunk(int id){
	char theChar;
	int i, j, sum, startLine, endLine;
	int local_line_sum[NUMBER_LINES / NUM_THREADS];
	
	#pragma omp private(id, theChar, i,j,sum,startLine,endLine,local_line_sum)
    {	
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
			local_line_sum[i - startLine] = sum;
		}

		// Critical Section
		#pragma omp critical
		{
			for(i = startLine; i < endLine; i++) {
				if(i != 0) line_sum[i-1] -= local_line_sum[i-startLine];
				if(i != NUMBER_LINES-1) line_sum[i] += local_line_sum[i-startLine];
				// printf("\t%d: %d\n", i, local_line_sum[i-startLine]);
			}

			// printf("\n");
		}
	}
}

void print_results(){
	int i;
	
	for(i = 0; i < NUMBER_LINES - 1; i++){
		printf("%d-%d: %d\n",i,i+1,line_sum[i]);
	}
}

main(int argc, char *argv[]){
	if(NUM_THREADS > NUMBER_LINES) {
		printf("Number of chunks must be less than number of lines\n");
		return -1;
	}

    omp_set_num_threads(NUM_THREADS);

	init_arrays();

	#pragma omp parallel
    {
        count_chunk(omp_get_thread_num());
    }

	print_results();
}

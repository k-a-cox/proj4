#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 1
#define NUMBER_LINES 1000000
#define MAX_LINE_SIZE 2005 //It seems the max line size is 2001, but that is only a guess

pthread_mutex_t mutexsum;                       // mutex for line_sum

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
    pthread_mutex_lock (&mutexsum);

	for(i = startLine; i < endLine; i++) {
		if(i != 0) line_sum[i-1] -= local_line_sum[i-startLine];
		if(i != NUMBER_LINES-1) line_sum[i] += local_line_sum[i-startLine];
		// printf("\t%d: %d\n", i, local_line_sum[i-startLine]);
	}
	printf("\n");
    
    pthread_mutex_unlock (&mutexsum);

    pthread_exit(NULL);

}

void print_results(){
	int i;
	
	for(i = 0; i < NUMBER_LINES - 1; i++){
		printf("%d-%d: %d\n",i,i+1,line_sum[i]);
	}
}

main(){
	int i, rc;
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	void *status;


	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


	
	if(NUM_THREADS > NUMBER_LINES) {
		printf("Number of chunks must be less than number of lines\n");
		return -1;
	}

	init_arrays();
       
	   
	for (i = 0; i < NUM_THREADS; i++ ) {
		rc = pthread_create(&threads[i], &attr, count_chunk, (void *)i);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	
 /* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	for(i=0; i<NUM_THREADS; i++) {
		rc = pthread_join(threads[i], &status);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}

	print_results();

	pthread_mutex_destroy(&mutexsum);
	printf("Main: program completed. Exiting.\n");
	pthread_exit(NULL);

}

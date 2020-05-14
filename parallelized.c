#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_CHUNKS 1
#define NUMBER_LINES 7
#define MAX_LINE_SIZE 500000000 //Somehow we don't want to overflow on characters per line of main file

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
		
		printf("%d: %s", i, char_array[i]);
	}
	fclose(file);
	printf("\n");
}

void count_chunk(int id){
	char theChar;
	int i, j, sum, startLine, endLine;
	int local_line_sum[NUMBER_LINES / NUMBER_CHUNKS];
	
	
	sum = 0;
	startLine = id * (NUMBER_LINES / NUMBER_CHUNKS);
	endLine = startLine + (NUMBER_LINES / NUMBER_CHUNKS);
	if (id == NUMBER_CHUNKS - 1) endLine = NUMBER_LINES;
	
	// Ready for parallelization

	printf("ID: %d\n", id);
	printf("Start line: %d\n", startLine);
	printf("End line: %d\n", endLine - 1);

	for(i = startLine; i < endLine; i++) {
		sum = 0;
		for(j = 0; j < MAX_LINE_SIZE; j++){
			theChar = char_array[i][j];
			if(theChar == '\0')
				break;
			sum += theChar;
		}
		local_line_sum[i - startLine] = sum;
	}
	
	// Critical Section
	for(i = startLine; i < endLine; i++) {
		if(i != 0) line_sum[i-1] -= local_line_sum[i-startLine];
		if(i != NUMBER_LINES-1) line_sum[i] += local_line_sum[i-startLine];
		printf("\t%d: %d\n", i, local_line_sum[i-startLine]);
	}

	printf("\n");
}

void print_results(){
	int i;
	
	for(i = 0; i < NUMBER_LINES - 1; i++){
		printf("%d-%d: %d\n",i,i+1,line_sum[i]);
	}
}

main(int argc, char *argv[]){
	if(NUMBER_CHUNKS > NUMBER_LINES) {
		printf("Number of chunks must be less than number of lines\n");
		return -1;
	}
	int i;

	init_arrays();
	
	for(i = 0; i < NUMBER_CHUNKS; i++){
		count_chunk(i);
	}
	print_results();
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_CHUNKS 3
#define NUMBER_LINES 3
#define MAX_LINE_SIZE 500000000 //Somehow we don't want to overflow on characters per line of main file

// Contains the output
char char_array[NUMBER_LINES][MAX_LINE_SIZE];
int line_sum[NUMBER_LINES - 1];

void init_arrays(char* filename){
	int i;
	
	for (i = 0; i < NUMBER_LINES - 1; i++){
		line_sum[i] = 0;
	}
	
	//Read in the main file into char_array
	FILE* file = fopen(filename, "r");
	for(i = 0; i < NUMBER_LINES; i++) {
		fgets(char_array[i], MAX_LINE_SIZE - 1, file);
		
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
			if(theChar == '\n')
				break;
			sum += theChar;
		}
		local_line_sum[i - startLine] = sum;
	}
	
	// Critical Section
	for(i = startLine; i < endLine; i++) {
		if(i != 0) line_sum[i-1] -= local_line_sum[i-startLine];
		if(i != NUMBER_LINES-1) line_sum[i] += local_line_sum[i-startLine];
		printf("\t%d: %d\n\n", i, local_line_sum[i-startLine]);
	}
}

void print_results(){
	int i;
	
	for(i = 0; i < NUMBER_LINES - 1; i++){
		printf("%d-%d: %d\n",i,i+1,line_sum[i]);
	}
}

main(int argc, char *argv[]){
	if(argc != 2) {
		printf("Requires a filename argument\n");
		return -1;
	}
	if(NUMBER_CHUNKS > NUMBER_LINES) {
		printf("Number of chunks must be less than number of lines\n");
		return -1;
	}
	char* filename = argv[1];
	int i;

	printf("%s\n", filename);
	init_arrays(filename);
	
	for(i = 0; i < NUMBER_CHUNKS; i++){
		count_chunk(i);
	}
	print_results();
}
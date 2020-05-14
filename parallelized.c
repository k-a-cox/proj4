#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_CHUNKS 4
#define NUMBER_LINES 16
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
		
	}
	fclose(file);
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
	for(i = startLine; i < endLine; i++) {
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
		if(i != 0) line_sum[i-1] -= local_line_sum[i];
		if(i != NUMBER_LINES-1) line_sum[i] += local_line_sum[i];
	}
}

void print_results(){
	int i, diff = 0;
	
	for(i = 1; i < NUMBER_LINES; i++){
		diff = line_sum[i] - line_sum[i-1];
		printf("%d-%d: %d/n",i-1,i,diff);
	}
}

main(int argc, char *argv[]){
	if(argc != 1) return -1;
	
	char* filename = argv[0];
	init_arrays(filename);
	
	for(int i = 0; i < NUMBER_CHUNKS; i++){
		count_line(i);
	}
	print_results();
}
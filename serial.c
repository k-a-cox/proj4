#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_LINES 16
#define BIG_NUMBER 500000000 //Somehow we don't want to overflow on characters per line of main file

char char_array[NUMBER_LINES][BIG_NUMBER];
int line_sum[NUMBER_LINES];

void init_arrays(){
	//Read in a block of the main file into char_array
	
	
	
	for (int i = 0; i < NUMBER_LINES; i++){
		line_sum[i] = 0;
	}
}

void count_line(int line){
	int i, sum;
	sum = 0;
	char theChar;
	
	for(int i = 0; i < BIG_NUMBER; i++){
		theChar = char_array[line][i];
		if(theChar == '\n')
			break;
		sum += theChar;
	}
	
	line_sum[line] = sum;
}

void print_results(){
	int i, diff = 0;
	
	for(i = 1; i < NUMBER_LINES; i++){
		diff = line_sum[i] - line_sum[i-1];
		printf("%d-%d: %d/n",i-1,i,diff);
	}
}

main(){
	int i;
	
	init_arrays();
	for(i = 0; i < NUMBER_LINES; i++){
		count_line(i);
	}
	print_results();
}
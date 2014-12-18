//
// sortfile.c
//
// This program reads in a list of numbers, prints it to stdout, uses
// bubble sort to sort it, prints it  again, and then writes it to the
// specified output file.
//
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "fileio.h"

//Function prototypes

void printArray(int array[], int size, bool writeHex);
void bubbleSort(int array[], int size, bool isIncreasing);
void sort(int* first, int* second, bool isIncreasing);
void swap(int* first, int* second);

int main(int argc, char **argv) {

	//const int MAX_FILENAME_LEN = 256;
	const int MAX_NUMBERS     = 100;

	char *inFilename = 0;
	char *outFilename = 0;
	int *numbers; //number array: to be dynamically allocated
	int count;
	int exitValue = -1;
	bool readHex = false;
	bool writeHex = false;
	int opt;
	int long_index = 0;
	static struct option long_options[] = {
		{"input", required_argument, 0, 'i'},
		{"output", required_argument, 0, 'o'},
		{"hexin", optional_argument, 0, 'x'},
		{"hexout", optional_argument, 0, 'y'},
		{0,0,0,0}
	};

	//Scan arguments to get input file name and output file name
	while ((opt = getopt_long(argc, argv, "i:o:xy", long_options, &long_index)) != -1){
		switch (opt){
			case 'i': 
				inFilename = optarg;
				break;
			case 'o':
				outFilename = optarg;
				break;
			case 'x':
				readHex = true;
				break;
			case 'y':
				writeHex = true;
				break;
		}
	}

	//printf("Input File is: %s \n", inFilename);
	//printf("Output File is: %s \n", outFilename);

	//printf("Enter the input file name; ");
	//scanf("%s", inFilename);

	// TODO: allocate memory for "numbers" array holding at most MAX_NUMBERS
	numbers = (int *) malloc(MAX_NUMBERS);

	count = readNumbers(numbers, inFilename, readHex);

	if (count >= 0) {
		//printf("Enter the output file name (will be created/overwritten): ");
		//scanf("%s", outFilename); 

		printArray(numbers, count, writeHex);
		bubbleSort(numbers, count, true);

		printArray(numbers, count, writeHex);
		writeNumbers(numbers, count, outFilename, writeHex);

		exitValue = 0;
	}

	free(numbers);

	return exitValue;

}



//
// This function prints the first 'size' elements from the array 'array' to
// stdout using printf
//
// preconditions:
// 	- array' is a valid array with >= 'size' elements
//
// postconditions:
// 	- Contents of 'array' are printed to standard output
//
void printArray(int array[], int size, bool writeHex) {

	if (size > 0) {
    	int i;

    	// TODO: print the array
    	printf("{");
    	for (i=0; i <size; i++){
    		if (writeHex)
    			printf ("%x,", array[i]);
    		else
    			printf("%d,", array[i]);
    	}
    	printf("}\n");

	}
}

//
// This function implements the bubble sort algorithm
//
// preconditions:
//	- 'array' is an array with at least 'size' elements
//
// postconditions:
//	- If isIncreasing is true, then the array will be sorted
//  	into non-decreasing order.  If it is false,
//  	then the array will be sorted into non-increasing order
//
void bubbleSort(int array[], int size, bool isIncreasing) {
	if (size >= 2) {
    	int i;

    	for (i = 0; i < size; i++) {
        	int j;

        	for (j = 0; j < size - 1 - i; j++) {
            	sort(&array[j], &array[j + 1], isIncreasing);
        	}
    	}
	}
}

//
// Sort will interchange the values pointed to by 'first' and 'second' if
// they aren't in the correct order as specified by 'dir'
//
// preconditions:
// 	- integer pointers 'first' and 'second' are non-NULL
//
// postconditions:
// 	- If isIncreasing is true then 'second' will be >= 'first'
// 	- If isIncreasing is false then 'second' will be <= 'first'
//
void sort(int* first, int* second, bool isIncreasing) {
	if (    	( (!isIncreasing) && (*first < *second) )
         	|| ( ( isIncreasing) && (*first > *second) ) ) {
    	swap(first, second);
	}
}

//
// Swaps the values pointed to by the parameters 'first' and 'second'
//
// preconditions:
// 	- Neither 'first' nor 'second' are NULL
//
// postconditions:
// 	- The variable pointed to by 'first' has the initial value of
//   	the variable pointed to by 'second', and vice versa.
//
void swap(int* first, int* second) {
	int holder;

	holder  = *first;
	*first  = *second;
	*second = holder;
}


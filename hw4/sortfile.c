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

//Function prototypes

void printArray(int array[], int size);
int  readNumbers(int array[], char* fname);
void writeNumbers(int array[], int size, char* fname);
void bubbleSort(int array[], int size, bool isIncreasing);
void sort(int* first, int* second, bool isIncreasing);
void swap(int* first, int* second);

int main(void) {
	const int MAX_FILENAME_LEN = 256;
	const int MAX_NUMBERS     = 100;

	char inFilename[MAX_FILENAME_LEN];
	char outFilename[MAX_FILENAME_LEN];
	int *numbers; //number array: to be dynamically allocated
	int count;
	int exitValue = -1;

	printf("Enter the input file name; ");
	scanf("%s", inFilename);

	// TODO: allocate memory for "numbers" array holding at most MAX_NUMBERS
	numbers = (int *) malloc(MAX_NUMBERS);

	count = readNumbers(numbers, inFilename);

	if (count >= 0) {
		printf("Enter the output file name (will be created/overwritten): ");
		scanf("%s", outFilename); 

		printArray(numbers, count);
		bubbleSort(numbers, count, true);

		printArray(numbers, count);
		writeNumbers(numbers, count, outFilename);

		exitValue = 0;
	}

	free(numbers);

	return exitValue;

}

//
// This function tries to open the file whose name is contained in the
// string variable 'fname' and read its contents into the array 'array'.  
//
// preconditions:
//	- 'array' is an integer array large enough to hold all of the
//  	elements in the file and 'fname' represents a valid filename
//
// postconditions:
//	- If 'fname' can be opened then its contents are read into
//  	the array 'array', the function returns the number of elements
//  	read.  If 'fname' is invalid an error message is printed and
//  	-1 is returned.
//

int readNumbers(int array[], char* fname) {
	// TODO: will you read the comments above this function carefully???
	// TODO: note those pre- and post-conditions!
	int numberRead = 0;
	FILE* fp;

	// Use fopen to try to open the file for reading
	// TODO:
	fp = fopen(fname, "r");
    
	// Test to see if the file was opened correctly
	// TODO:
	if (fp == NULL){
		perror("Invalid File Input");
		return -1;
	}

	// Now read until end of file
	// TODO:
	int count = 0;

	while (fscanf(fp, "%d", &array[count]) != EOF){
		numberRead += 1;
		count += 1;
	}

	// Close the file pointer
	// TODO:
	fclose(fp);

	// Return the number of items read
	return numberRead;  // can it be negative? if in doubt, read.
}


//
// This function writes the first 'size' numbers from array 'array' to a file
// whose name is contained in 'fname'.
//
// preconditions:
//	- 'array' is an array of integers equal to or larger than
//  	size and 'fname' holds a valid filename
//
// postconditions:
//	- If a file can be created then 'size' elements from
//  	a are written to that file and it is closed.  If
//  	the file cannot be created then an error message is printed.
//
void writeNumbers(int array[], int size, char* fname) {
	// TODO: will you read the comments above?
	int i;
	// Declare local variables and FILE pointer
	FILE *fp;

	// Use fopen to try to open the file for writing
	// TODO:
	fp = fopen(fname, "w");

	// Test to see if the file was opened correctly
	// TODO:
	if (fp == NULL){
		perror("Could not write to file.");
	}


	// use fprintf to write to the file
	// TODO:
	for (i = 0; i < size; i++){
		fprintf(fp, "%d\n", array[i]);
	}

	// close the file
	// TODO:
	fclose(fp);
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
void printArray(int array[], int size) {
	if (size > 0) {
    	int i;

    	// TODO: print the array
    	printf("{");
    	for (i=0; i <size; i++){
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


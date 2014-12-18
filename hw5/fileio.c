#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "fileio.h"

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

int readNumbers(int array[], char* fname, bool readHex) {
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

	if (readHex){
		while (fscanf(fp, "%x", &array[count]) != EOF){
			numberRead += 1;
			count += 1;
		}
	}
	else{
		while (fscanf(fp, "%d", &array[count]) != EOF){
			numberRead += 1;
			count += 1;
			if (count >= 100){
				printf("\nThe file provided contains something other than decimal numbers. If it contains hex numbers, use the -x flag.\n\n");
				exit(0);
			}
		}
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
void writeNumbers(int array[], int size, char* fname, bool writeHex) {
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
		if (writeHex)
			fprintf(fp, "%x\n", array[i]);
		else
			fprintf(fp, "%d\n", array[i]);
	}

	// close the file
	// TODO:
	fclose(fp);
}
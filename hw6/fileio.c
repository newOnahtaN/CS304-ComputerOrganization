#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
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

int readNumbers(int **array, char* fname, bool readHex) {
	int numberRead = 0;
	int fileDescriptor;

	fileDescriptor = open(fname, O_RDONLY);
    
	// Test to see if the file was opened correctly
	// TODO:
	if (fileDescriptor  == -1){
		perror("Invalid File Input");
		return -1;
	}

	*array = malloc(100 * sizeof(int *));
	int limit = 100;

	// Now read until end of file
	char *buffer;
	buffer = (char *) malloc(512 * sizeof(char));
	int buffLen;
	int count = 0;

	while ((buffLen = read(fileDescriptor, buffer, 512)) > 0){

		int i = 0;
			
		while (i < buffLen){

			//create a temp str to hold single number
			//get a char from the buffer
			//if the char does not equal some sort of white space - add numbers into the string array 
			char str[10];
			memset(&str[0], 0, sizeof(str));
			char cha = buffer[i];
			int j = 0;
			int lenStr = 0;


			if ((cha != ' ' && cha != '\n' && cha != '\t')){
				while ((cha != ' ' && cha != '\n' && cha != '\t') &&  (i < buffLen)){
					str[j] = cha;
					j += 1;
					i += 1;
					cha = buffer[i];
					lenStr += 1;
				}

				//convert the string to an integer value
				//add it tinto the number array 
				int integer;

				if (readHex){
					if ((lenStr >2) && (str[0] == '0') && (str[1] == 'x')){
						integer =  (int) strtol(str, NULL, 0);
					}
					else{
						integer = (int) strtol(str, NULL, 16);
					}
				}
				else
					integer = atoi(str);

				

				*(*array + count) = integer;
				count += 1;
				numberRead += 1;

				if (numberRead == limit){
					*array = realloc(*array, (limit + 50) * sizeof(int *));
					limit = limit +50;
				}
			}
			else{
				i += 1;
			}
		}
	}

	// Close the file pointer
	close(fileDescriptor);

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
	int i = 0;
	
	int fd;
	char buf[256], sep = '\n', sign = '-';
	int k, n, m, useSign = 0;

	//PERMISSIONS ARE THEIR OWN SEPERATE ARGUMENT
	fd = open(fname,O_CREAT|O_TRUNC|O_WRONLY, S_IRWXO|S_IRUSR|S_IWUSR);

	for (i = 0; i<size; i++){
		n = array[i];
		k = 255;

		//Account for negatives
		if (n < 0){
			n = abs(n);
			useSign = 1;
		}

		if (writeHex == false){ //decimal
			do {
				m = n % 10;
				buf[k] = m + '0';
				k--;
				n = n/10;
			
			}while (n > 0);
		}else{ //hexadecimal
			do {
				m = n % 16;
				if (m >= 10){
					buf[k] = m - 10 + 'A';
				}else{
					buf[k] = m + '0';
				}
				k--;
				n = n / 16;
			}while (n > 0);
		}
		
		
		if (useSign == 1){
			write(fd, &sign, 1);
			useSign = 0;
		}
		write(fd, buf+k+1, 255-k);	
		write(fd, &sep, 1);
	}
	close(fd);
	/*
	int i;
	int fileDescriptor;

	// Use fopen to try to open the file for writing
	fileDescriptor = open(fname, O_WRONLY | O_TRUNC |  O_CREAT, S_IRUSR | S_IWUSR | S_IRWXO);

	// Test to see if the file was opened correctly
	if (fileDescriptor  == -1){
		perror("Invalid File Input");
	}


	// use fprintf to write to the file
	// TODO:

	if (writeHex){
		char arr[512];

		for (i = 0; i < size; i++){
			memset(arr, 0, sizeof(arr));
			sprintf(arr, "%x\n", array[i]);
			write(fileDescriptor, arr, sizeof(arr));
		}

	}


	else{
		char arr[512];

		for (i = 0; i < size; i++){
			memset(arr, 0, sizeof(arr));
			sprintf(arr, "%d\n", array[i]);
			write(fileDescriptor, arr, sizeof(arr));
		}
	}

	// close the file
	// TODO:
	close(fileDescriptor);
	*/
}

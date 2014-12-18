#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

//fileio.h header file
//prototypes for the readnumbers and writenumbers functions
//Nathan Owen, CS 304

int  readNumbers(int **array, char* fname, bool readHex);
void writeNumbers(int array[], int size, char* fname, bool writeHex);

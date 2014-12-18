#include "binprint.h"
#include <stdio.h>
#include <math.h>

//Nathan Owen, ncowen@email.wm.edu
//CS 304 Computer Organization
//Homework #2 Part One


/* Implements the functions in binprint.h, meant to calculate two's complement for different values*/


void bin_int(int ii)
{
	int bitsize, temp;
	int size = 1;
	size = sizeof(size);

	for(bitsize = (size*8) - 1; bitsize >= 0; bitsize--)
	{
		temp = ii >> bitsize;

		if (temp & 1)
			printf("1");
		else
			printf("0");

	}

	printf("\n");

}

void bin_byte(char ch)
{
	int bitsize, temp;
	char size = 'a';
	size = sizeof(size);

	for(bitsize = (size*8) - 1; bitsize >= 0; bitsize--)
	{
		temp = ch >> bitsize;

		if (temp & 1)
			printf("1");
		else
			printf("0");

	}

	printf("\n");
}
void bin_short(short int si)
{
	int bitsize, temp;
	short int size = 1;
	size = sizeof(size);

	for(bitsize = (size*8) - 1; bitsize >= 0; bitsize--)
	{
		temp = si >> bitsize;

		if (temp & 1)
			printf("1");
		else
			printf("0");

	}

	printf("\n");
}
void bin_long(long int li)
{
	int bitsize, temp;
	long int size = 1;
	size = sizeof(size);

	for(bitsize = (size*8) - 1; bitsize >= 0; bitsize--)
	{
		temp = li >> bitsize;

		if (temp & 1)
			printf("1");
		else
			printf("0");

	}

	printf("\n");	
}

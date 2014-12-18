#include "binprint.h"
#include "binprint.c"
#include <stdio.h>


//Nathan Owen, ncowen@email.wm.edu
//CS 304 Computer Organization
//Homework #2 Part One


/* Tests the functions in binprint.c, meant to calculate two's complement for different values*/
int main(){

	//  Test Code

	bin_int(-125);
	bin_int(125);
	printf("\n");

	bin_byte('a');
	bin_byte('\n');
	printf("\n");

	bin_short(7);
	bin_short(-7);
	printf("\n");

	bin_long(1001);
	bin_long(-1001);

	//


	return 0;

}
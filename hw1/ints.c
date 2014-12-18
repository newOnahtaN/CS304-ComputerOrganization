//
// Program: ints.c
// Student: Nathan Owen
//
// The reason why 32767 + 1 yields -32768 is very similar to the reason as to why 65535 + 1 was zero in the last problem.
// If you'll notice, 32767 is rougly half of 65535, an this is becuase in this case, the numbers here are also represented
// as 16 bit shorts, but they are signed.  Because they are signed, half of the available numbers that can possibly be
// represented are dedicated to numbers that are negative.  This means that a signed short has a range of -32768 to 32767 
// rather than 0 to 65535.  
//
// Perhaps less obviously, the reason why adding one to 32767 yields -32768 instead of zero as it did previously is because 
// of the way that the hardware interprets these signed binary numbers. The computer uses a system called Two's complement,
// and because of the way that binary numbers are read in this method, when a binary number reaches a certain point where the
// leftmost bit is a one, overflow occurs. This overflow simply means that at this point, the interpreter 
// designates this number and all those above it as negative numbers.  After that, the numbers progress up from -32768 to
// the highest binary number possible in 16 bit two's complement (-1).  When it reaches that point, in the same way as the last
// problem, the number wraps back down to 0 after further addition.
//

#include <stdio.h>

int main()
{
    short int ovi;
    short int ovi_plus_1 = ovi + 1;

    // assume this code is for an machine architecture
    // where a short int is 16-bit
    ovi = 32767;
    ovi_plus_1 = ovi + 1;

    printf("ovi is an int and its value is %i\n", ovi);
    printf("ovi plus one is %i\n", ovi_plus_1);
    printf("\n");

    printf("Does the above output makes sense to you?\n");
    printf("Explain it.\n\n");

    return 0;
}
//
// Program: uints.c
// Student: Nathan Owen
/*
// 65535 is the highest number that can be represented in 16 bits, and therefore, when 1 is added to this number of type
// short which is composed of 16 bits, it cannot represent the next logical number (65536) and instead wraps back around
// to its starting point - 0.
*/ 

#include <stdio.h>

int main()
{
    unsigned short int ovi;     
    unsigned short int ovi_plus_1;

    // assume this code is for an machine architecture
    // where a short int is 16-bit
    ovi = 65535;
    ovi_plus_1 = ovi + 1;

    printf("ovi is an unsigned int and its value is %u\n", ovi);
    printf("ovi plus one is %u\n", ovi_plus_1);
    printf("\n");

    printf("Does the above output makes sense to you?\n");
    printf("Explain it.\n\n");

    return 0;
}

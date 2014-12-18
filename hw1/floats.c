//
// Program: floats.c
// Student: Nathan Owen
//
// In attempting to figure out why this program has this result, I learned the following things.
// The float values in C are calculated by an industry standard known as IEEE 754-1985. In this method of
// float computation, doubles (for double precision), are accurate to approximately 15 digits.  Floats are not completely
// accurate because of the limitations of computer hardware - most fratctional numbers are approximations.
// The method is very good at rounding however, so in most cases it will appear as though floats are entirely accurate
// when in reality their actual values are off by very miniscule amounts because of the way they are represented in binary.
// This causes the effect in the program seen below where two seemingly equal numbers are off by a billionth of a digit,
// and therefore are not perfectly equal.  Modern programming languages have methods to circumvent this problem, but the
// problem itself exists to this day.
// 
// Floats are calculated by reading binary code as parts of an exponential expression.
//

#include <stdio.h>

int main()
{
    double a;
    double b;
    double c;

    a = 0.3;
    b = 0.6;
    c = 0.9;

    printf("a = %.100f, \n\n b = %.100f, \n\n a + b = %.100f\n\n", a, b, a+b);
    printf("c = %.100f\n", c);

    if (a + b == c) {
        printf("Yes, the if statement confirms a + b is equal to c.\n");
        printf("Great if this happens.\n\n");
    } else {
        printf("No, the if statement says a + b is not equal to c. Why?\n");
        printf("Hint: make printf to print more digits after the decimal\n");
        printf("      points to reveal the true values of a, b, and a+b.\n\n");
    }

    return 0;
}

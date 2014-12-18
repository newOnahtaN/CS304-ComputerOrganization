/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* We do support the IEC 559 math functionality, real and complex.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
  /* 
   * Hex for all even bits is 55555555, so we use & to set the odd bits to zero.
   * The result will be either the correct result, or not.  To do this, we compare
   * it to the result we want using the same method as isEquals - XOR with a logical not.
   */
   int evenBits = (0x55 << 24) | (0x55 << 16) | (0x55 << 8) | (0x55);
	return !((x & evenBits) ^ evenBits);
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
 /*
  *Understanding this process is dependent on the principle that the arithmetic negation of zero is zero
  *in two's complement.  What is meant by this is that if the ~ operation plus one is used on 
  *any normal number, it will result in that number's negative. In the case of zero, it does not
  *end up negative, but rather as zero again.  With this notion, we can find the negation of any number, and
  *there is a guarantee that for all of these pairs, one of the two numbers is negative, and hence has its
  *most significant bit as a 1. Using the | operator on the members of the pair, the result will always have its most 
  *significant bit as a 1 as well.  We can then shift this to the least significant bit, negate it and return it
  *as the result.  In the case of zero, the pair is zero and zero as we said before, instead of a positive and
  *negative number, and the result of 0|0 is zero.  It will also be shifted and negated, and give the opposite return 
  *value.
  */
  x = x | (~x + 1);
  x = ~(x >> 31);
  x &= 1;
  return x;

}
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
 /*
  * I constructed this tautology using a truth table, guess
  * and check, and intuition. 
  */
	return ~((~x) | (~y));
}
/* 
 * bitMask - Generate a mask consisting of all 1's 
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
 /*
  * Creates a mask of ones below lowbit and a mask of ones above the high bit.
  * These two are then combined using the 'or' operator and negated so that only
  * the zeroes between the two masks should be turned to ones.
  */
  
  int MSB = ~(1 << 31); //Yields 0x7fffffff
  int LSB = ~1; // Yileds 0xfffffffe
 
  int lowMask = ((MSB) >> (31 + (~lowbit + 1)));
  int highMask = ((LSB) << highbit);
  
  return ~(highMask | lowMask);
}
/* 
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
 /*
  * Moves the least significant bit all the way to the left (most significant)
  * and then moves it back. If one, all will become ones, if zero, all will 
  * become zeroes.
  */
  x = x << 31;
  x = x >> 31;
  return x;
}
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
 /* 
  * This is the specific value the function is asking for composed with the single byte 55 or 0101
  * in each byte postion. 
  */
  return (0x55 << 24) | (0x55 << 16) | (0x55 << 8) | (0x55);
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
 /* 
  * Shifts the integer given to the right until the byte desired is the last byte,
  * then shifts all the way back to the beginning as the first byte. Sets the left
  * three bytes to zero using the & operator.
  */
  x = x << (25 + (~(n << 3)));  // 24 - the number times 8 (<< 3) (combined the +1 into 24)
  x = x >> 24;
  x &= 0x000000ff;
  return x;
}
/* 
 * leastBitPos - return a mask that marks the position of the
 *               least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2 
 */
int leastBitPos(int x) {
 /* 
  * The negative number combined using & with the number itself will yield
  * only the least significant digit.
  */
  int negX = ~(x) + 1;
  return x & negX;
}
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
 /*
  *Understanding this process is dependent on the principle that the arithmetic negation of zero is zero
  *in two's complement.  What is meant by this is that if the ~ operation plus one is used on 
  *any normal number, it will result in that number's negative. In the case of zero, it does not
  *end up negative, but rather as zero again.  With this notion, we can find the negation of any number, and
  *there is a guarantee that for all of these pairs, one of the two numbers is negative, and hence has its
  *most significant bit as a 1. Using the | operator on the members of the pair, the result will always have its most 
  *significant bit as a 1 as well.  We can then shift this to the least significant bit, negate it and return it
  *as the result.  In the case of zero, the pair is zero and zero as we said before, instead of a positive and
  *negative number, and the result of 0|0 is zero.  It will also be shifted and negated, and give the opposite return 
  *value.
  */
  x = x | (~x + 1);
  x = ~(x >> 31);
  x &= 1;
  return x;

}
/* 
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
 /*
  * Returns the specific value asked for. ~0 is 0xffffffff
  */
  return ~0;
}
/* 
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
 /*
  * Puts one in the most significant bit space and then negates it.
  */
  return ~(1 << 31);
}
/* 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
  return !(x ^ y);
}
/* 
 * isNegative - return 1 if x < 0, return 0 otherwise 
 *   Example: isNegative(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNegative(int x) {
 /*
  * Shifts the most significant bit all the way to the right, then sets all other 
	* bits to zero. The result is either 0 or 1.
  */
  x = (x >> 31);
  return x & 1;
}
/* 
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4 
 */
int isNonZero(int x) {
 /*
  * Using the methods above and below, we reimplemented and simply made the 
  * assertion that if either return true, then this function must return false.
  * As a matter of fact, for this question, we could also use the same code for 
  * bang() and logicalNegation() above, these are all equivalent.  This is just a 
  * different implementation.
  */

  int isNegative = ((x >> 31) & 0x00000001);

  int isPositive = (((x + ~0) >> 31) & 0x00000001) ^ 1;

  return isNegative | isPositive;


}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
 /*
  * We first subtract 1 to make zero have it's most significant bit as a 1.
  * Then, shift the most significant bit to the zero position and sets all other postions to zero.
  * If the value is one, it is negative, if zero, it was positive. We negate to return correctly.
  */
 
  //Alternate solution as used in NonZero()
  //x += ~0 ; // Add negative one
  //x = (x >> 31);
  //x &= 1;
  //return !x;
  
  int originalInverse = !x;
  x &= (1 << 31); // Make x either 0x80000000 or 0x00000000
  x = (x >> 31); //Shift MSB to the LSB
  x |= originalInverse;
  return !x;
  

}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
 /*
  * In two's complement, negation is just bitwise negation plus one by definition.
  */
  return ~(x) + 1;
}
/* 
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) {
 /*
  * First, we find the most signficant bit (msb). Then, we use !! to have all
  * positive and negative numbers set to 1, and 0 set to 0. At that point, we
  * add the negation of the msb twice to our !!x because if x was a positive number
  * or zero, the msb will be zero. If it was a negative number, then msb is 1
  * and adding its negation twice will subtract two and yield -1 as it should.
  */
    int msb = (x >> 31); //As we found in similar functions, this find the
    msb &= (0x00000001); //most significant bit.

    return (!!x) + (~(msb) + 1) + (~(msb) + 1); 

}
/* 
 * sm2tc - Convert from sign-magnitude to two's complement
 *   where the MSB is the sign bit
 *   Example: sm2tc(0x80000005) = -5.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int sm2tc(int x) {
 /*
  * If a negative number in signed magnitude is passed into this function, the msb will be 1's and the choice mask
  * 0's.  It will also mean that the conversion acts as 0x80000000 minus the number itself, which is actually the 
  * definition of conversion from signed magnitude to 2's complement. That result is then &'d with the msb which 
  * will yield that number again (because the msb is 1's). This is then |'d with the choiceMask which is all zeroes,
  * and will again yield with the same number.
  *
  * Alternatively, if a positive number is thrown in, to prevent conversion, the msb is all zeroes and the choiceMask becomes the original
  * number.  The conversion variable will then return zero because the msb (which is &'d last) is all zeroes. Because
  * the choiceMask (the original number) is |'d with the conversion (which is zeroes), the result will be the original 
  * number, and no conversion occurs.
  */
  int highBit = (1 << 31); //Just creating the number 0x80000000

  int msb = (x >> 31);
  int choiceMask = (~msb & x); //Will either be all 1's or all 0's
  int conversion = (((x & highBit) + (~x + 1)) & msb);
  return choiceMask | conversion;

}
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
 /*
  *First step is to switch the sign bit, then check to see if the input is NaN. If it is,
  *just return that same number. If it isn't, return the result.
  */
  unsigned t = uf & 0x7fffffff;

  if (t > 0x7f800000) // its a NaN
    return uf;

  
  return t;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  /*
   *The main idea here is incrementing the exponent by one.  In order to do this, I had to include a lot
   * of checks to handle cases for NaN and Infinite floats.  
   */

  unsigned exponent = (uf >> 23) & 0xff;
  unsigned sign = uf & 0x80000000;
  unsigned fraction = uf & 0x007fffff;

  if (exponent == 255 || (exponent == 0 && fraction ==0))
    return uf;  // Check if 

  if (exponent){
    exponent++;
  }
  else if (fraction == 0x7fffff) {
    fraction--;
    exponent++;
  }
  else{
    fraction <<= 1;
  }

  return (sign) | (exponent << 23) | (fraction);

}

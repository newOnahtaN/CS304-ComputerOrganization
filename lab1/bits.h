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
int allEvenBits();
int test_allEvenBits();
int bang(int);
int test_bang(int);
int bitAnd(int, int);
int test_bitAnd(int, int);
int bitMask(int, int);
int test_bitMask(int, int);
int copyLSB(int);
int test_copyLSB(int);
int evenBits();
int test_evenBits();
int getByte(int, int);
int test_getByte(int, int);
int leastBitPos(int);
int test_leastBitPos(int);
int logicalNeg(int);
int test_logicalNeg(int);
int minusOne();
int test_minusOne();
int tmax();
int test_tmax();
int isEqual(int, int);
int test_isEqual(int, int);
int isNegative(int);
int test_isNegative(int);
int isNonZero(int);
int test_isNonZero(int);
int isPositive(int);
int test_isPositive(int);
int negate(int);
int test_negate(int);
int sign(int);
int test_sign(int);
int sm2tc(int);
int test_sm2tc(int);
unsigned float_abs(unsigned);
unsigned test_float_abs(unsigned);
unsigned float_twice(unsigned);
unsigned test_float_twice(unsigned);

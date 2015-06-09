/* 
 * CS:APP Data Lab 
 * 
 * Hendry Gurung 930767414
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
*This return 1 if all even-numbered bits in word set to 1
*/
	int y = (85<<24)+(85<<16)+(85<<8)+(85);
	return !((x&y)^y);
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
*To replicate the ! operator we must display a 1 or zero depending on the value of the bit, 1 for non-zero, and zero otherwise. 
*The initial integer is shifted right by decreasing powers of 2, starting with 2^4, and ending with 2^0. 
*The opposite of this value plus 1 will result in either a 1 or 0 in each position.
*/	
	x = ( x >> 16 ) | x;
	x = ( x >> 8 ) | x;
	x = ( x >> 4 ) | x;
	x = ( x >> 2 ) | x;
	x = ( x >> 1) | x;
	int final = ~x & 1;
	return final;
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
*Taking the or value of the NOT for each variable, and the NOT of the result, allows us to get the equivalent of and. 
*This premise follows the concept of DeMorgan's law
*/
	return ~ (~x | ~y);
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
*To accurately generate a mask of 1's between the highbit and hte lowbit, we first left shift a series of 1's by the highbit, in shiftOne. 
*We then left shift a series of 1's by the lowbit, and take the inverse of that value, giving us shiftTwo. Next we left shift a single 1 by the highbit and take the inverse of that value, giving us shiftThree.
*Using the AND operator with shiftOne and shiftThree provides the right values for 1's and the final OR operator gives us the right mask.	
*/
	int shiftOne = ~0 << highbit;
	int shiftTwo = ~(~0 << lowbit);
	int shiftThree = ~(1 << highbit);
	shiftOne = shiftOne & shiftThree;
	int final =  ~(shiftOne | shiftTwo);
	return final;
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
*By taking the AND value of 1, we can find the least significant bit-to fill out the rest of the bits we take the NOT of that value and add 1
*/
	int leastSig = (x & 1); 
	int notSig = ~leastSig;
	notSig += 1;
	return notSig;
}
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
/*
*return word with all even-numbered bits set to 1
*/
	int byte = 0x55;
	int word = byte | byte<<8;
	return word | word<<16;
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
* To get the desired Byte, we shift the hexadecimal number right by 8 times n, the desired bit then take the AND value of the resulting bit and 0xFF.
*/	
	int andInt = 0xFF;
	return (x >> 8*n) & andInt;
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
*To determine the least significant bit we find the negative two's complement representation then use the AND operator to single out the least significant bit
*/
	int negX = ~x+1;
	int leastBit = negX & x;  
	return leastBit;
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
*implement the ! operator, using all of the legal operators except !
*/
	return ((~(~x+1)&(~x))>>31)&1;
}
/* 
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
/*
*Returns a value of -1
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
*By left shifting 1 and taking the bitwise not value of the resulting number we are able to produce the highest possible value 
*of a two's complement, 0x7FFFFFF 
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
/*
*The first step we take here is to use the exclusive OR on the two integers, which results in all zeros if they are the same. 
*We then use the logical NOT which returns 1 if A is non-zero, which only occurs if the exclusive OR returns a non-zero value 
*/
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
*To determine whether the given integer is negative or not, we shift the bits all the way to the right, leaving us with the sign bit alone
*We then take the NOT value of the sign bit twice, in order to fit the requirement of the isNegative function 
*/
	int shifted = !!(x >> 31);
	return shifted;
}
/* 
 * isNonZero - Check whether x is nonzero using the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4 
 */
int isNonZero(int x) {
/*
*Check whether x is nonzero using the legal operators except !
*/
	int h = ~x + 1;
	return ((h >> 31) | (x >> 31)) & 1;
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
*Use 1<<31 to mask sign bit and return 1 if x > 0, return 0 otherwise 
*/
	int hen = !((1 << 31) & x);
	int zero = !x;
	return hen ^ zero;
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
*Return -x 
*/
	return (~x) + 1;
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
*return 1 if positive, 0 if zero, and -1 if negative
*/
	return (x > 0) - (x < 0);
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
*If MSB is 0, do nothing. If not, the smallest number has the magnitude of the complement of its bit pattern
*viewed as a two's complement number plus the smallest two's possible two's complement number plus one. 
*/
	int initialShift = 1 << 31;
	int negativeX = (initialShift & x) >> 31;
	int positiveX = ~negativeX;
	int sign = initialShift & x;
	positiveX = positiveX & x;
	negativeX = negativeX & x;
	negativeX = ~(negativeX + ~0);
	return (negativeX + positiveX + sign);
}
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for floating point argument f.
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
*Return bit-level equivalent of absolute value of f for floating point argument f.
*/
	unsigned guru = 0x7FFFFFFF;		
	unsigned NaN = 0x7F800001;
	unsigned hen = uf & guru;		
	if (hen >= NaN){
    	return uf;
	}
	else{
    	return hen;
	}
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for floating point argument f.
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
*Return bit-level equivalent of expression 2*f for floating point argument f.
*/
	unsigned nosign = uf << 1;
	unsigned sign = uf & 0x80000000;
	if (nosign >= 0xFF000000) { // NaN and infinity, return itself
		return uf;
	} 
	else if (nosign < 0x01000000) { // Denormalized, return non-sign right shift 1 bit
		return sign | nosign;
	}
	else if (nosign < 0xFE000000) { // Normalized, non-sign < 0xFE000000, return exp + 1
		return uf + 0x800000;
	} 
	else { // Other situation, return infinity
		return sign | 0x7F800000;
	}
}

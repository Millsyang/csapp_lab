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
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


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

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

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
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
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
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~(~(~x & y) & ~(x & ~y));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1 << 31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  /* only 2*Tamx+1 == -1 and 2*0xfffffffff+1 == -1*/
  return !!~x & !(~(x+x+1));
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  /* x | 0x55555555 == 0xffffffff*/
  return !(~(0x55 | 0x55 << 8 | 0x55 << 16 | 0x55 << 24 | x));
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {

  return ~x+1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /* if it is positive, then x-0x30 is positive and 0x39-x positive */
  return !((x>>31) | (x+(~0x30+1))>>31 | (0x39+(~x+1))>>31);
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /* if x==0,make multi_n == 0x0,else make multi_n == 0xffffffff*/
  int multi_n = ((!!x) << 31) >> 31;
  return ((~multi_n) & z) + (multi_n & y);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /*if x < 0 and y > 0,return 1 else if xy>=0,and y-x>=0 return 1*/
  int sign_x = x >> 31;
  int sign_y = y >> 31;
  //  return ((~(((y+(~x+1)) >> 31)) & ~(sign_x ^ sign_y)) | flag) & 1;
   return ~(((((y + (~x + 1)) >> 31)) | (sign_x ^ sign_y)) & (~sign_x|sign_y)) & 1;
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  /* only 0 and -0's sign bit are both 0*/
  return ~(x >> 31 | (~x+1) >> 31) & 1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x)
{
  /* 看完大神们的题解，我的代码是真的僵硬。。。*/
  /* if x <0 , sign_x = 0xffffffff,else 0x0 */
  int sign_x = (x >> 31);
  int true_x = (sign_x & ~x) + (~sign_x & x);
  int z_flag = ~((!!true_x) << 31 >> 31) /*0xffffffff if true_x is 0 ,else 0*/;
  /* binary search */
  int beg = 0, en = 31;
  int mid = 15;
  int flag = ((true_x + ~(1 << 15) + 1) >> 31); /*0 if true_x - 2**res > 0 else 0xffffffff*/
  beg = (mid & ~flag) + (beg & flag);
  en = (mid & flag) + (en & ~flag);
  mid = ((beg + en) >> 1); 

  flag = ((true_x + ~(1 << mid) + 1) >> 31);
  beg = (mid & ~flag) + (beg & flag);
  en = (mid & flag) + (en & ~flag);
  mid = ((beg + en) >> 1); 

  flag = ((true_x + ~(1 << mid) + 1) >> 31);
  beg = (mid & ~flag) + (beg & flag);
  en = (mid & flag) + (en & ~flag);
  mid = ((beg + en) >> 1); 

  flag = ((true_x + ~(1 << mid) + 1) >> 31);
  beg = (mid & ~flag) + (beg & flag);
  en = (mid & flag) + (en & ~flag);
  mid = ((beg + en) >> 1);

  flag = ((true_x + ~(1 << mid) + 1) >> 31);
  mid = (mid + (beg & flag) + (en & ~flag)) >> 1;

  return (z_flag & 1) + (~z_flag & (mid + 2));
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  unsigned exp = (uf >> 23) & 0xff; //get exponent
  unsigned frac = uf & 0x7fffff;
  if (exp == 0xff && frac) //exp==0xff and frac!=0 means uf is NaN
    return uf;
  if(!exp)             //if exp is 0,then frac*2
    frac = frac << 1;
  else                 //else exp+1
  {
    exp ++;
    if (exp & 0x100)    //exp overflow
    {
      exp = 255;
      frac = 0;
    }
  }
  return (uf & 0x80000000)+(exp<<23)+frac;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf)
{
  /*这里一开始理解错了舍入规则，所以判断条件有点小毛病，但不影响结果*/
  unsigned intBits;
  unsigned firstDecimal;
  int sign = uf & 0x80000000;
  unsigned exp = (uf >> 23) & 0xff; //get exponent
  unsigned frac = uf & 0x7fffff;
  unsigned frac_plus1 = frac | 0x800000;
  unsigned _bias = exp - 125;
  intBits = (frac_plus1 << 7) >> (32 - _bias);
  if (exp >= 158) //exp-127 >= 31
    return 0x80000000;
  else if (exp < 126) //exp-127 < -1
    return 0;
  else if (_bias <= 24)
  {
    firstDecimal = (frac_plus1 >> (24 - _bias)) & 1;
    /*only watch the first bit after decimal points when rounding*/
    intBits += (firstDecimal & (intBits & 1));
  }
  return sign ? (~intBits + 1) : intBits;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    if(x > 127)  // too big
      return 0x7f800000;
    else if(x < -149)  // too small
      return 0;
    else if(x < -126)  //unnormalized
      return 1 << (x + 149);
    else               //normalized
      return (x + 127) << 23; 
}

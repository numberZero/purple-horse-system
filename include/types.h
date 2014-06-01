#pragma once
#ifdef X86_64

typedef unsigned long long u16;
typedef   signed long long s16;
typedef unsigned long  u8;
typedef   signed long  s8;
typedef unsigned int   u4;
typedef   signed int   s4;
typedef unsigned short u2;
typedef   signed short s2;
typedef unsigned char  u1;
typedef   signed char  s1;

typedef u16 udword;
typedef s16 sdword;
typedef u8 uword;
typedef s8 sword;
typedef u4 uhword;
typedef s4 shword;
typedef u1 ubyte;
typedef s1 sbyte;

#else

typedef unsigned long long u8;
typedef   signed long long s8;
typedef unsigned int   u4;
typedef   signed int   s4;
typedef unsigned short u2;
typedef   signed short s2;
typedef unsigned char  u1;
typedef   signed char  s1;

typedef u8 udword;
typedef s8 sdword;
typedef u4 uword;
typedef s4 sword;
typedef u2 uhword;
typedef s2 shword;
typedef u1 ubyte;
typedef s1 sbyte;

#endif

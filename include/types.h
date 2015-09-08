#pragma once
#define packed_struct struct __attribute__((packed))
#define packed_union union __attribute__((packed))

#ifdef X86_64

typedef unsigned long long u16, uint128_t;
typedef signed long long s16, int128_t;
typedef unsigned long u8, uint64_t;
typedef signed long s8, int64_t;
typedef unsigned int u4, uint32_t;
typedef signed int s4, int32_t;
typedef unsigned short u2, uint16_t;
typedef signed short s2, int16_t;
typedef unsigned char u1, uint8_t;
typedef signed char s1, int8_t;

typedef u16 udword;
typedef s16 sdword;
typedef u8 uword, uintptr_t, size_t;
typedef s8 sword, intptr_t, ssize_t, ptrdiff_t;
typedef u4 uhword;
typedef s4 shword;
typedef u1 ubyte;
typedef s1 sbyte;

#else

typedef unsigned long long u8, uint64_t;
typedef signed long long s8, int64_t;
typedef unsigned int u4, uint32_t;
typedef signed int s4, int32_t;
typedef unsigned short u2, uint16_t;
typedef signed short s2, int16_t;
typedef unsigned char u1, uint8_t;
typedef signed char s1, int8_t;

typedef u8 udword;
typedef s8 sdword;
typedef u4 uword, uintptr_t, size_t;
typedef s4 sword, intptr_t, ssize_t, ptrdiff_t;
typedef u2 uhword;
typedef s2 shword;
typedef u1 ubyte;
typedef s1 sbyte;

#endif

typedef char c1, char_t, char8_t;
typedef char16_t c2;
typedef char32_t c4;

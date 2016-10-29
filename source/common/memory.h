#pragma once
#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

void *memset(void *s, u1 c, size_t n);
void *memsetb(void *s, u1 c, size_t n);
void *memsetw(void *s, u2 c, size_t n);
void *memsetl(void *s, u4 c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);

#ifdef __cplusplus
}
#endif

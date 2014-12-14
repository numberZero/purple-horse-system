#pragma once

typedef unsigned long size_t;
typedef long ssize_t;

#ifdef __cplusplus
extern "C"
{
#endif

void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);

#ifdef __cplusplus
}
#endif

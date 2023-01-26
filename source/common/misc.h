#pragma once

#ifdef __cplusplus
extern "C"
#endif
void halt() __attribute__((noreturn));

static inline void _die(char const *text, long N) __attribute__((noreturn));
static inline void _die(char const *text, long N)
{
	asm volatile ("cli");
	struct t
	{
		char c;
		unsigned char s;
	} *buf = (t*)0x000B8000;
	for(long i = 0; i < N; ++i)
	{
		buf[i].c = text[i];
		buf[i].s = 0xCF;
	}
	halt();
}
#define	die(message)	_die(message, sizeof(message) - 1)

template <typename T>
static inline T const& EnsureRange(T const& value, T const& min, T const& max)
{
	if(value < min)
		return min;
	if(value > max)
		return max;
	return value;
}

static inline void delay() {
	asm ("pause");
}

static inline void sleep() {
	asm ("hlt");
}

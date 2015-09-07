#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

void halt() __attribute__((noreturn));

#ifdef __cplusplus
}
#endif

static void _die(char const *text, long N) __attribute__((noreturn));
static void _die(char const *text, long N)
{
	struct t
	{
		char c;
		unsigned char s;
	} *buf = (t*)0x000B8000;
	for(long i = 0; i < N; ++i)
	{
		buf[i].c = text[i];
		buf[i].s = 0x0C;
	}
	halt();
}
#define	die(message)	_die(message, sizeof(message))

template <typename T>
static inline T const& EnsureRange(T const& value, T const& min, T const& max)
{
	if(value < min)
		return min;
	if(value > max)
		return max;
	return value;
}

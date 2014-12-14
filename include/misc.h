#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

void halt() __attribute__((noreturn));

#ifdef __cplusplus
}
#endif

static void kprint(char const *text, long N)
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
}

template <typename T>
static void die(T text) __attribute__((noreturn));

template <typename T>
static void die(T text)
{
	kprint(text, sizeof(text));
	halt();
}

template <typename T>
static inline T const& EnsureRange(T const& value, T const& min, T const& max)
{
	if(value < min)
		return min;
	if(value > max)
		return max;
	return value;
}

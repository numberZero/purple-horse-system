#pragma once
#include "types.h"

union __attribute__((packed)) terminal_symbol
{
	u2 value;
	struct
	{
		char symbol;
		u1 style;
	};
};

class mmapped_nano_terminal
{
private:
	terminal_symbol* data;
	long width;
	long height;
	
public:
	
	terminal_symbol* get(long x, long y)
	{
		if(x < 0)
			return nullptr;
		if(y < 0)
			return nullptr;
		if(x >= width)
			return nullptr;
		if(y >= height)
			return nullptr;
		return data + y * width + x;
	}
	
	terminal_symbol& operator() (long x, long y)
	{
		return *get(x, y);
	}
};

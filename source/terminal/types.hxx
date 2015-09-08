#pragma once
#include "types.h"

enum Color
{
	Black		= 0b0000,
	Red		= 0b0100,
	Green		= 0b0010,
	Brown		= 0b0110,
	Blue		= 0b0001,
	Purple		= 0b0101,
	Teal		= 0b0011,
	Gray		= 0b0111,
	Light		= 0b1000,
	LightRed	= 0b1100,
	LightGreen	= 0b1010,
	Yellow		= 0b1110,
	LightBlue	= 0b1001,
	Magenta	= 0b1101,
	Cyan		= 0b1011,
	White		= 0b1111,
	
	Dark		= Black,
	DarkGray	= Light + Black,
	LightGray	= Gray,
	Fuchsia	= Magenta
};

packed_struct Style
{
	union
	{
		u1 style;
		struct
		{
			Color foreground:4;
			Color background:4;
		};	
	};
	
	Style() = default;
	Style(u1 s): style(s) {}
	Style(Color fg, Color bg): foreground(fg), background(bg) {}
	Style& operator= (u1 s) { style = s; }
	operator u1() { return style; }
};

packed_union Symbol
{
	u2 value;
	packed_struct 
	{
		char symbol;
		u1 style;
	};
	
	Symbol() = default;
	explicit constexpr Symbol(u2 _value): value(_value) {}
	explicit constexpr Symbol(char _symbol = ' ', u1 _style = 0x70): symbol(_symbol), style(_style) {}
	constexpr operator u2() const { return value; }
};

struct TermPos
{
	long x;
	long y;
	
	TermPos() = default;
	TermPos(long _x, long _y): x(_x), y(_y) {}
};

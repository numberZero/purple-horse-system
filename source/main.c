#include "memory.h"
#include "port.h"

struct __attribute__((packed)) term_sym
{
	char code;
	unsigned char color;
};

struct term_sym* terminal = (struct term_sym*)0x000B8000; // real-mode address for VGA terminal
long term_sx = 80;
long term_sy = 25;
long term_cx = 0;
long term_cy;

void clearscreenex(char ch, unsigned char color)
{
	struct term_sym s;
	long i;
	s.code = ch;
	s.color = color;
	for(i = 0; i != term_sx * term_sy; ++i)
		terminal[i] = s;
}

void clearscreen()
{
	clearscreenex(' ', 0x07);
}

struct term_sym* getrealchar(long x, long y)
{
	return terminal + x + y * term_sx;
}

void setchar(char ch, long x, long y)
{
	getrealchar(x, y)->code = ch;
}

void setcharex(char ch, unsigned char color, long x, long y)
{
	struct term_sym* rc = getrealchar(x, y);
	rc->code = ch;
	rc->color = color;
}

void updatecursor()
{
	u2 l = term_cy * 80 + term_cx;
	outb(0x3D4, 14);
	outb(0x3D5, l >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, l);
}

void setcursorpos(long x, long y)
{
	term_cx = x;
	term_cy = y;
	updatecursor();
}

void scroll()
{
	memmove(terminal, terminal + term_sx, term_sx * (term_sy - 1) * 2);
	--term_cy;
	updatecursor();
}

void newline()
{
	term_cx = 0;
	if(++term_cy >= term_sy)
		scroll();
	else
		updatecursor();
}

void putcharnc(char ch)
{
	setchar(ch, term_cx, term_cy);
	if(++term_cx >= term_sx)
		newline();
}

void putchar(char ch)
{
	setchar(ch, term_cx, term_cy);
	if(++term_cx >= term_sx)
		newline();
	else
		updatecursor();
}

void putstring(char const* str)
{
	while(*str)
		putcharnc(*(str++));
	updatecursor();
}

void putstringexpand(char const* str)
{
	while(*str)
	{
		switch(*str)
		{
			case '\r':
				term_cx = 0;
				break;
			case '\n':
				while(term_cx)
					putcharnc(' ');
				break;
			case '\t':
				do
				{
					putcharnc(' ');
				} while(term_cx % 8);
				break;
			default:
				putchar(*str);
		}
		++str;
	}
	updatecursor();
}

int main(void *mboot_ptr)
//int main(struct multiboot *mboot_ptr)
{
	clearscreenex('Z', 0x0A);
	while(1)
	{
		setcursorpos(0, 0);
		putstringexpand("Hello world!\n123456789\n\t90 degrees\nxx\t.0.al.\t=)\n:D nZero\n");
	}
	return 0xDEADBABA;
}

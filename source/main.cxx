#include "desctables.h"
#include "memory.h"
#include "port.h"
#include "terminal/console.hxx"

extern SGDTEntry gdt[];

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

extern "C" void clearscreen()
{
	clearscreenex(' ', 0x07);
}

extern "C" void __cxa_pure_virtual()
{
	die("Pure virtual function call");
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

static const char hexdigits[17] = "0123456789ABCDEF";

extern "C" int kernel_main(void *mboot_ptr) __attribute__((noreturn));

extern "C" int kernel_main(void *mboot_ptr)
//int main(struct multiboot *mboot_ptr)
{
	KTerminal term;
	KTerminal::terminal = &term; // yes term is local, but actually kernel_main never returns, so that’s OK
	KConsole con;
	KConsole::console = &con;
	con.setStyle({LightGreen, Black});
	clearscreenex(' ', 0x00);
	con.writeLine("Console works");
	init_descriptor_tables();
	con.writeLine("Descriptor tables initialized");
	con.write(hexdigits[gdt[1].access_byte >> 4]);
	con.write(hexdigits[gdt[1].access_byte & 15]);
	con.write(' ');
	con.write(hexdigits[gdt[1].granularity_byte >> 4]);
	con.write(hexdigits[gdt[1].granularity_byte & 15]);
	con.write(' ');
	while(1)
	{
//		con.setStyle({});
//		con.setCursorPos(TermPos(0, 0));
		//putstringexpand("Hello world!\n123456789\n\t90 degrees\nxx\t.0.al.\t=)\n:D nZero\n");
		//halt();
	}
	die("kernel_main must not return");
	//return 0xDEADBABA;
}

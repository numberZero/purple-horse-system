#include "desctables.h"
#include "memory.h"
#include "port.h"
#include "terminal/console.hxx"

extern SGDTEntry gdt[];
extern SIDTEntry idt[];

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
/*
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
*/

extern "C" int kernel_main(void *mboot_ptr) __attribute__((noreturn));
/*
extern "C" unsigned char isr_1;
extern "C" unsigned char isr_2;
extern "C" unsigned char isr_8;
extern "C" unsigned char isr_10;
*/
extern "C" void *code;
extern "C" void *data;
extern "C" void *bss;
extern "C" void *heap;
extern "C" void *getStackPointer();
extern "C" void *getCodePointer();
/*
void PrintInterruptHandler(long num, unsigned char *isr)
{
	kout->write("Interrupt handler (", num, "):");
	for(unsigned char *v(isr); *v != 0x90; ++v)
	{
		kout->write(" ");
		kout->writeValue(*v, 16, 2);
	}
	kout->writeLine();
}
*/
extern "C" int kernel_main(void *mboot_ptr)
//int main(struct multiboot *mboot_ptr)
{
	KTerminal term;
	KTerminal::terminal = &term; // yes term is local, but actually kernel_main never returns, so that’s OK
	KConsole con;
	KConsole::console = &con;
	con.setStyle({LightGreen, Black});
	clearscreenex(' ', 0x00);
	kout = &con;
	kout->writeLine("Console works");
	init_descriptor_tables();
	for(long i = 0; i < 5; ++i)
	{
		kout->write("Segment ", i, " flags: ");
		kout->writeValue(gdt[i].access_byte, 16);
		kout->write(" ");
		kout->writeValue(gdt[i].granularity_byte, 16);
		kout->writeLine();
	}
	kout->writeLine(mboot_ptr);
/*	for(long i = 0; i < 32; ++i)
	{
		kout->write("Interrupt ", i, " flags: ");
		kout->writeValue(idt[i].flags, 16);
		kout->write(", segment: ");
		kout->writeValue(idt[i].segment, 16);
		kout->write(", handler: ", idt[i].GetBase());
		kout->writeLine();
	}
	PrintInterruptHandler(1, &isr_1);
	PrintInterruptHandler(2, &isr_2);
	PrintInterruptHandler(8, &isr_8);
	PrintInterruptHandler(10, &isr_10);*/
	kout->writeLine("Code section: ", &code);
	kout->writeLine("Data section: ", &data);
	kout->writeLine("BSS section: ", &bss);
	kout->writeLine("Heap section: ", &heap);
	kout->writeLine("Stack pointer: ", getStackPointer());
	kout->writeLine("Code pointer: ", getCodePointer());
	asm volatile ("int $3");
	asm volatile ("int $32");
	kout->writeLine("Entering interrupt-driven mode...");
	while(1)
	{
		asm volatile ("hlt");
		kout->writeLine("hlt returns");
//		kout->setStyle({});
//		kout->setCursorPos(TermPos(0, 0));
		//putstringexpand("Hello world!\n123456789\n\t90 degrees\nxx\t.0.al.\t=)\n:D nZero\n");
		//halt();
	}
	die("kernel_main must not return");
	//return 0xDEADBABA;
}

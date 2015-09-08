#include "desctables.h"
#include "memory.h"
#include "port.h"
#include "terminal/console.hxx"

extern SGDTEntry gdt[];
extern SIDTEntry idt[];

extern "C" void __cxa_pure_virtual()
{
	die("Pure virtual function call");
}

extern "C" void *code;
extern "C" void *data;
extern "C" void *bss;
extern "C" void *heap;
extern "C" void *getStackPointer();
extern "C" void *getCodePointer();

extern "C" int __attribute__((noreturn)) kernel_main(void *mboot_ptr)
{
	KTerminal term;
	KTerminal::terminal = &term; // yes term is local, but actually kernel_main never returns, so that’s OK
	KConsole con;
	KConsole::console = &con;
	con.setStyle({LightGreen, Black});
	con.clearEx(' ', 0x00);
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
	kout->writeLine("Code section: ", &code);
	kout->writeLine("Data section: ", &data);
	kout->writeLine("BSS section: ", &bss);
	kout->writeLine("Heap section: ", &heap);
	kout->writeLine("Stack pointer: ", getStackPointer());
	kout->writeLine("Code pointer: ", getCodePointer());
	kout->writeLine("Entering interrupt-driven mode...");
	while(1)
	{
		asm volatile ("hlt");
	}
	die("kernel_main must not return");
}

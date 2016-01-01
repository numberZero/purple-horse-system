#include "memory.h"
#include "port.h"
#include "mboot.hxx"
#include "desctables.hxx"
#include "terminal/console.hxx"
#include "codegen/instructions.hxx"
#include "memory/kmalloc.hxx"

extern SGDTEntry gdt[];
extern SIDTEntry idt[];

extern "C" void __cxa_pure_virtual()
{
	die("Pure virtual function call");
}

extern "C" u1 code;
extern "C" u1 data;
extern "C" u1 bss;
extern "C" u1 heap;
extern "C" u1 end;
extern "C" void *getStackPointer();
extern "C" void *getCodePointer();

extern "C" int __attribute__((noreturn)) kernel_main(SMultibootInfo *mboot)
{
#ifdef VIDEO_STACK
	CDummyConsole con;
#else
	KTerminal term;
	KTerminal::terminal = &term; // yes term is local, but actually kernel_main never returns, so that’s OK
	KConsole con;
	KConsole::console = &con;
#endif
	KAllocator alloc(&end);
	KAllocator::allocator = &alloc;
	kout = &con;
 	con.setStyle({LightGreen, Black});
	con.clearEx(' ', Style(Color::LightBlue, Color::Black));
	kout->writeLine("Console enabled");
	kout->writeLine("Kernel allocator enabled");
	kout->writeLine("Multiboot: ", mboot);
	kout->write("Multiboot flags: ");
	kout->writeValue(mboot->flags, 2);
	kout->writeLine();
	if(mboot->flag.memory_info)
	{
		kout->writeLine("Lower memory: ", mboot->mem_lower);
		kout->writeLine("Upper memory: ", mboot->mem_upper);
	}
	if(mboot->flag.memory_map)
	{
		kout->writeLine(mboot->mmap_length);
	}
	init_descriptor_tables();
	for(long i = 0; i < 5; ++i)
	{
		kout->write("Segment ", i, " flags: ");
		kout->writeValue(gdt[i].access_byte, 16);
		kout->write(" ");
		kout->writeValue(gdt[i].granularity_byte, 16);
		kout->writeLine();
	}
	kout->writeLine("Code section: ", &code);
	kout->writeLine("Data section: ", &data);
	kout->writeLine("BSS section: ", &bss);
	kout->writeLine("Heap section: ", &heap);
	kout->writeLine("End of sections: ", &end);
	kout->writeLine("Stack pointer: ", getStackPointer());
	kout->writeLine("Code pointer: ", getCodePointer());
	kout->writeLine("Entering interrupt-driven mode...");
	asm volatile ("sti");
	while(1)
	{
		asm volatile ("hlt");
	}
	die("kernel_main must not return");
}

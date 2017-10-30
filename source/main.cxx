#include "memory.h"
#include "misc.h"
#include "hardware/mboot.hxx"
#include "terminal/console.hxx"
// #include "codegen/instructions.hxx"
#include "interrupts/handling.hxx"
#include "interrupts/irq.hxx"
#include "memory/init.hxx"
#include "memory/kmalloc.hxx"

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
	KAllocator alloc((void *)0x00100000);
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
	memory_initialization_facility = new(undeletable) CMemoryInitializationFacility();
	for(size_t i = 0; i < memory_initialization_facility->entries; ++i)
	{
		kout->write("Segment ", i, " (", 8 * i, ") flags: ");
		kout->writeValue(memory_initialization_facility->entry(i).access_byte, 16);
		kout->write(" ");
		kout->writeValue(memory_initialization_facility->entry(i).granularity_byte, 16);
		kout->writeLine();
	}
	kout->writeLine("Code section: ", &code);
	kout->writeLine("Data section: ", &data);
	kout->writeLine("BSS section: ", &bss);
	kout->writeLine("Heap section: ", &heap);
	kout->writeLine("End of sections: ", &end);
	kout->writeLine("Stack pointer: ", getStackPointer());
	kout->writeLine("Code pointer: ", getCodePointer());
	kout->writeLine("Initializing interrupt handling...");
	interrupt_handling_facility = new(undeletable) CInterruptHandlingFacility();
	kout->writeLine("Starting system timer...");
	irq_handling_facility->start_timer();
	kout->writeLine("Entering interrupt-driven mode...");
	asm volatile ("sti");
	for(;;)
	{
		asm volatile ("hlt");
	}
	die("kernel_main must not return");
}

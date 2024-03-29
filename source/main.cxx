#include "memory.h"
#include "misc.h"
#include "hardware/mboot.hxx"
#include "terminal/console.hxx"
// #include "codegen/instructions.hxx"
#include "interrupts/handling.hxx"
#include "interrupts/irq.hxx"
#include "memory/init.hxx"
#include "memory/kmalloc.hxx"
#include "multitask/basic.hxx"
#include "terminal/serial.hxx"

extern "C" void __cxa_pure_virtual()
{
	die("Pure virtual function call");
}

extern "C" u1 code[];
extern "C" u1 data[];
extern "C" u1 bss[];
extern "C" u1 heap[];
extern "C" u1 end[];
extern "C" void *getStackPointer();
extern "C" void *getCodePointer();

int counter = 0;
static char const tmpl[] = "=== Task ___ ===";

void test1(void *text)
{
	for (int k = 0; k != 50; ++k) {
		kout->writeLine((char const *)text);
		sleep();
	}
	char *buf = reinterpret_cast<char *>(KAllocator::allocator->alloc(sizeof(tmpl)));
	memcpy(buf, tmpl, sizeof(tmpl));
	buf[11] = '0' + (counter % 10);
	buf[10] = '0' + (counter / 10 % 10);
	buf[9] = '0' + (counter / 100 % 10);
	++counter;
	Scheduler::scheduler->create_task(test1, buf);
}

extern "C" int __attribute__((noreturn)) kernel_main(SMultibootInfo *mboot)
{
#ifdef VIDEO_STACK
	CDummyConsole con;
#else
	KTerminal term;
	KTerminal::terminal = &term; // yes term is local, but actually kernel_main never returns, so that’s OK
	KConsole con;
	KConsole::console = &con;
	SerialConsole scon{0x3F8};
#endif
	KAllocator alloc(&heap);
	KAllocator::allocator = &alloc;
	kout = &con;
 	con.setStyle({LightGreen, Black});
	con.clearEx(' ', Style(Color::LightBlue, Color::Black));
	kout->writeLine("Console enabled");
	kout->writeLine("Kernel allocator enabled");
	if (mboot) {
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
	} else {
		kout->writeLine({Yellow, Black}, "Got no Multiboot data");
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
	kout->writeLine("Starting threading facility...");
	Scheduler::scheduler = new(undeletable) Scheduler();
	kout->writeLine("Starting some threads...");
	Scheduler::scheduler->create_task(test1, (void *)"=== Task 1 === Hello, world!");
	Scheduler::scheduler->create_task(test1, (void *)"=== Task 2 === Welcome to the Userspace!");
	Scheduler::scheduler->create_task(test1, (void *)"=== Task 3 === At ring 0 :P");
	kout->writeLine("Entering interrupt-driven mode...");
	asm volatile ("sti");
	for(;;)
	{
		asm volatile ("hlt");
	}
	die("kernel_main must not return");
}

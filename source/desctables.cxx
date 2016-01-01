#include "desctables.hxx"
#include "types.h"
#include "memory.h"
#include "port.h"
#include "terminal/console.hxx"
#include "codegen/instructions.hxx"
#include "memory/kmalloc.hxx"
#define	GDT_ITEMS	5
#define	IDT_ITEMS	256

using interrupt_handler_t = void *;

typedef struct registers
{
	u4 ds;                  // Data segment selector
	u4 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
	u4 int_no, err_code;    // Interrupt number and error code (if applicable)
	u4 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

extern "C"
{
	u2 gdt_flush(SGDTPointer *);
	void idt_flush(SIDTPointer *);
	void interrupt_handler(registers_t regs);
	void irq_handler(registers_t regs);
	
	extern u1 const irq_handler_wrapper[];
	extern u1 const interrupt_handler_wrapper[];
}

packed_struct SInterruptHandler
{
	SInterruptHandler(u1 int_id, bool ec_passed) :
		cli(codegen::FlagInterrupt::Opcode::disallow),
		pushec(0),
		pushid(int_id),
		handle(interrupt_handler_wrapper)
	{
		if(ec_passed)
			pushec.disable();
	}
	
private:
	codegen::FlagInterrupt cli;
	codegen::Optional<codegen::PushByte> pushec;
	codegen::PushByte pushid;
	codegen::JumpLong handle;
	codegen::Nop nop;
};

static_assert(sizeof(SInterruptHandler) == 11, "Alignment?!");

packed_struct SIRQHandler
{
	SIRQHandler(u1 irq_id) :
		cli(codegen::FlagInterrupt::Opcode::disallow),
		pushirq(irq_id),
		pushint(irq_id + 32),
		handle(irq_handler_wrapper)
	{
	}
	
private:
	codegen::FlagInterrupt cli;
	codegen::PushByte pushirq;
	codegen::PushByte pushint;
	codegen::JumpLong handle;
	codegen::Nop nop;
};

static_assert(sizeof(SIRQHandler) == 11, "Alignment?!");

class CMemoryHandling
{
};

class CInterruptHandling
{
};

CInterruptHandling* ih;

// Internal function prototypes.
static void init_gdt();
static void init_idt();
static void initialize_pic();
static void start_timer();

SGDTEntry gdt[GDT_ITEMS];
SGDTPointer gdt_ptr;
SIDTEntry idt[IDT_ITEMS];
SIDTPointer idt_ptr;

void init_descriptor_tables()
{
	init_gdt();
	init_idt();
	start_timer();
}

static void init_gdt()
{
	kout->writeLine("Initializing GDT...");
	gdt_ptr.base = gdt;
	gdt_ptr.limit = sizeof(gdt) - 1;

	gdt[0].Clear();
	gdt[1].Setup(0, 0xFFFFFFFF, RingKernel, SGDTEntry::DescriptorDefault, SGDTEntry::SegmentCode);
	gdt[2].Setup(0, 0xFFFFFFFF, RingKernel, SGDTEntry::DescriptorDefault, SGDTEntry::SegmentData);
	gdt[3].Setup(0, 0xFFFFFFFF, RingUser, SGDTEntry::DescriptorDefault, SGDTEntry::SegmentCode);
	gdt[4].Setup(0, 0xFFFFFFFF, RingUser, SGDTEntry::DescriptorDefault, SGDTEntry::SegmentData);
	kout->writeLine("GDT initialized");

	long s = gdt_flush(&gdt_ptr);
	kout->writeLine("GDT loaded");
	kout->writeValue("Code segment is ");
	kout->writeValue(s);
	kout->writeLine();
}

static void init_idt()
{
	kout->writeLine("Initializing IDT...");
	idt_ptr.limit = sizeof(idt) - 1;
	idt_ptr.base = idt;

	memset(&idt, 0, sizeof(idt));
// Interrupt handlers
	for(int k = 0; k != 32; ++k)
	{
		idt[k].Setup(new(undeletable) SInterruptHandler(k, (k >= 8) && (k <= 13) && (k != 9)), 0x0008, RingKernel);
	}
// IRQ handlers
	for(int k = 0; k != 16; ++k)
	{
		idt[k + 32].Setup(new(undeletable) SIRQHandler(k), 0x0008, RingKernel);
	}
	
	idt[0x40].Setup(new(undeletable) SInterruptHandler(0x40, false), 0x0008, RingKernel);
	
	kout->writeLine("IDT prepared");
	initialize_pic();
	idt_flush(&idt_ptr);
	kout->writeLine("IDT loaded");
}

static void initialize_pic()
{
	kout->writeLine("Initializing PIC...");
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
	kout->writeLine("PIC initialized");
}

static void start_timer()
{
	outb(0x43, 0b00110010);
	outb(0x40, 0);
	outb(0x40, 0);
	u2 divisor = 1193180 / 19;

	// Send the command byte.
	outb(0x43, 0x36);

	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	u1 l = (u1)(divisor & 0xFF);
	u1 h = (u1)( (divisor>>8) & 0xFF );

	// Send the frequency divisor.
	outb(0x40, l);
	outb(0x40, h);
	kout->writeLine("Timer started");
}

void interrupt_handler(registers_t regs)
{
	kout->writeLine("Interrupt ", regs.int_no, " received");
}

void irq_handler(registers_t regs)
{
	if(regs.int_no != 32)
		kout->writeLine("IRQ ", regs.err_code, " interrupt ", regs.int_no, " received");
	if(regs.err_code == 1)
	{
		u1 v = inb(0x60);
		kout->writeLine("Keyboard input: ", v);
		if(v == 1) // escape
		{
			kout->setStyle({LightRed});
			kout->writeLine("Keyboard halt");
			die("System halted by user");
		}
	}
	if(regs.int_no >= 40)
		outb(0xA0, 0x20);
	outb(0x20, 0x20);
}

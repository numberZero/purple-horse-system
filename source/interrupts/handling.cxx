#include "handling.hxx"
#include "table.hxx"
#include "port.h"
#include "memory.h"
#include "misc.h"
#include "terminal/console.hxx"
#include "codegen/instructions.hxx"
#include "memory/kmalloc.hxx"
#define	IDT_ITEMS	256

CInterruptHandlingFacility *interrupt_handling_facility;

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
	void idt_flush(SIDTPointer *);
	void interrupt_handler(registers_t regs);
	extern u1 const interrupt_handler_wrapper[];
}

packed_struct SInterruptHandler_WithEC
{
	SInterruptHandler_WithEC(u1 int_id) :
		cli(codegen::FlagInterrupt::Opcode::disallow),
		pushid(int_id),
		handle(interrupt_handler_wrapper)
	{
	}

private:
	codegen::FlagInterrupt cli;
	codegen::PushByte pushid;
	codegen::JumpLong handle;
};

static_assert(sizeof(SInterruptHandler_WithEC) == 8, "Alignment?!");

packed_struct SInterruptHandler_NoEC
{
	SInterruptHandler_NoEC(u1 int_id) :
		cli(codegen::FlagInterrupt::Opcode::disallow),
		pushec(0),
		pushid(int_id),
		handle(interrupt_handler_wrapper)
	{
	}

private:
	codegen::FlagInterrupt cli;
	codegen::PushByte pushec;
	codegen::PushByte pushid;
	codegen::JumpLong handle;
};

static_assert(sizeof(SInterruptHandler_NoEC) == 10, "Alignment?!");

packed_struct SIRQHandler
{
	SIRQHandler(u1 irq_id) :
		cli(codegen::FlagInterrupt::Opcode::disallow),
		pushirq(irq_id),
		pushint(irq_id + 32),
		handle(interrupt_handler_wrapper)
	{
	}

private:
	codegen::FlagInterrupt cli;
	codegen::PushByte pushirq;
	codegen::PushByte pushint;
	codegen::JumpLong handle;
};

static_assert(sizeof(SIRQHandler) == 10, "Alignment?!");

void initialize_pic();

CInterruptHandlingFacility::CInterruptHandlingFacility()
{
	kout->writeLine("Initializing IDT...");
	idt_ptr.limit = sizeof(idt) - 1;
	idt_ptr.base = idt;

	memset(&idt, 0, sizeof(idt));
// Interrupt handlers
	for(int k = 0; k != 32; ++k)
	{
		if((k >= 8) && (k <= 13) && (k != 9))
			idt[k].Setup(new(undeletable) SInterruptHandler_WithEC(k), 0x0008, RingKernel);
		else
			idt[k].Setup(new(undeletable) SInterruptHandler_NoEC(k), 0x0008, RingKernel);
	}
// IRQ handlers
	for(int k = 0; k != 16; ++k)
	{
		idt[k + 32].Setup(new(undeletable) SIRQHandler(k), 0x0008, RingKernel);
	}

	idt[0x40].Setup(new(undeletable) SInterruptHandler_NoEC(0x40), 0x0008, RingKernel);

	kout->writeLine("IDT prepared");
	initialize_pic();
	idt_flush(&idt_ptr);
	kout->writeLine("IDT loaded");
}

void initialize_pic()
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

void start_timer()
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
	if((regs.int_no >= 32) && (regs.int_no < 48))
	{
		kout->writeLine("IRQ ", regs.err_code, " interrupt ", regs.int_no, " received");
		if(regs.int_no >= 40)
			outb(0xA0, 0x20);
		outb(0x20, 0x20);
	}
}
/*
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
*/

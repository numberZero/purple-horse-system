#include "handling.hxx"
#include "table.hxx"
#include "hardware/port.h"
#include "memory.h"
#include "misc.h"
#include "terminal/console.hxx"
#include "codegen/instructions.hxx"
#include "memory/kmalloc.hxx"
#include "irq.hxx"

CInterruptHandlingFacility *interrupt_handling_facility;

extern "C"
{
	void idt_flush(SIDTPointer *);
	void interrupt_handler(SInterruptHandlerArgument &regs);
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

CInterruptHandlingFacility::CInterruptHandlingFacility()
{
	kout->writeLine("Initializing IDT...");
	idt_ptr.limit = sizeof(idt) - 1;
	idt_ptr.base = idt;

	memset(&idt, 0, sizeof(idt));
	for(int k = 0; k != 8; ++k)
		idt[k].Setup(new(undeletable) SInterruptHandler_NoEC(k), 0x0008, RingKernel);
	idt[8].Setup(new(undeletable) SInterruptHandler_WithEC(8), 0x0008, RingKernel);
	idt[9].Setup(new(undeletable) SInterruptHandler_NoEC(9), 0x0008, RingKernel);
	for(int k = 10; k != 14; ++k)
			idt[k].Setup(new(undeletable) SInterruptHandler_WithEC(k), 0x0008, RingKernel);
	for(int k = 14; k != 32; ++k)
			idt[k].Setup(new(undeletable) SInterruptHandler_NoEC(k), 0x0008, RingKernel);
	for(int q = 0; q != 16; ++q)
		idt[q + 32].Setup(new(undeletable) SIRQHandler(q), 0x0008, RingKernel);
	idt[0x40].Setup(new(undeletable) SInterruptHandler_NoEC(0x40), 0x0008, RingKernel);

	kout->writeLine("IDT prepared");
	irq_handling_facility = new(undeletable) CIRQHandlingFacility();
	idt_flush(&idt_ptr);
	kout->writeLine("IDT loaded");
}

void CInterruptHandlingFacility::handle_interrupt(int id, u4 error, u2& data_segment, SCommonRegisters& registers, SInterruptRegisters& context)
{
	kout->writeLine("Interrupt ", id, " received");
}

void interrupt_handler(SInterruptHandlerArgument &regs)
{
	if((regs.int_no >= 32) && (regs.int_no < 48))
		irq_handling_facility->handle_irq(regs.err_code, regs.ds, regs.registers, regs.context);
	else
		interrupt_handling_facility->handle_interrupt(regs.int_no, regs.err_code, regs.ds, regs.registers, regs.context);
}

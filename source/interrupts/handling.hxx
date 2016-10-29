#pragma once
#include "types.h"
#include "table.hxx"

struct SInterruptHandlerArgument
{
	u4 ds; // Data segment selector
	u4 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
	u4 int_no, err_code; // Interrupt number and error code (if applicable)
	u4 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
};

class CInterruptHandlingFacility
{
public:
	static constexpr const size_t entries = 256;

private:
	SIDTEntry idt[entries];
	SIDTPointer idt_ptr;

public:
	CInterruptHandlingFacility();
};

extern CInterruptHandlingFacility *interrupt_handling_facility;

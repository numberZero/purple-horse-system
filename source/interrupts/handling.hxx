#pragma once
#include "table.hxx"

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

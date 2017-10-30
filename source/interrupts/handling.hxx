#pragma once
#include "types.h"
#include "table.hxx"
#include "hardware/types.hxx"

struct SInterruptHandlerArgument
{
	union
	{
		u2 ds;
		u4 ds_place;
	};
	SCommonRegisters registers;
	u4 int_no, err_code;
	SInterruptRegisters context;
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
	void handle_interrupt(int id, u4 error, u2 &data_segment, SCommonRegisters &registers, SInterruptRegisters &context);
};

extern CInterruptHandlingFacility *interrupt_handling_facility;

#pragma once
#include "handling.hxx"

class CIRQHandlingFacility
{
public:
	CIRQHandlingFacility();
	void start_timer();
	void handle_irq(int id, u2 &data_segment, SCommonRegisters &registers, SInterruptRegisters &context);
};

extern CIRQHandlingFacility *irq_handling_facility;

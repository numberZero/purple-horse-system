#pragma once

class CIRQHandlingFacility
{
public:
	CIRQHandlingFacility();
	void start_timer();
};

extern CIRQHandlingFacility *irq_handling_facility;

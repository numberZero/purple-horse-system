#include "irq.hxx"
#include "port.h"
#include "terminal/console.hxx"

CIRQHandlingFacility *irq_handling_facility;

CIRQHandlingFacility::CIRQHandlingFacility()
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

void CIRQHandlingFacility::start_timer()
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

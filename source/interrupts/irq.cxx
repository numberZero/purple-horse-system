#include "irq.hxx"
#include "misc.h"
#include "hardware/port.h"
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

void CIRQHandlingFacility::handle_irq(int id, u2 &data_segment, SCommonRegisters &registers, SInterruptRegisters &context)
{
	if(id != 0) // if not system timer
		kout->writeLine("IRQ ", id, " received");
	if(id == 1)
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
// restore PIC state
	if(id >= 8)
		outb(0xA0, 0x20);
	outb(0x20, 0x20);
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

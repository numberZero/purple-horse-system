#include "serial.hxx"
#include "misc.h"
#include "hardware/port.h"

enum class SerialConsole::Port {
	Data = 0, // DLAB=0
	InterruptEnable = 1, // DLAB=0
	DeviceControl = 2,
	LineControl = 3,
	ModemControl = 4,
	LineStatus = 5,
	ModemStatus = 6,
	Scratch = 7,

	DivisorLo = 0, // DLAB=1
	DivisorHi = 1, // DLAB=1
};

inline int SerialConsole::port(Port kind)
{
	return base_port + (int)kind;
}

SerialConsole::SerialConsole(int _base_port) :
	base_port(_base_port)
{
	static constexpr u1 test_value_1 = 0xC5;
	static constexpr u1 test_value_2 = 0xA9;
	static constexpr u1 test_value_3 = 0xB3;

	outb(port(Port::Scratch), test_value_1);
	if (inb(port(Port::Scratch)) != test_value_1) {
		die("Serial console initialization failed");
	}

	outb(port(Port::Scratch), test_value_2);
	if (inb(port(Port::Scratch)) != test_value_2) {
		die("Serial console initialization failed");
	}

	outb(port(Port::InterruptEnable), 0x00);    // Disable all interrupts
	outb(port(Port::LineControl), 0x80);    // Enable DLAB (set baud rate divisor)
	outb(port(Port::DivisorLo), 0x03);
	outb(port(Port::DivisorHi), 0x00);
	outb(port(Port::LineControl), 0x03);    // 8 bits, no parity, one stop bit
	outb(port(Port::DeviceControl), 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(port(Port::ModemControl), 0x0B);    // IRQs enabled, RTS/DSR set

	outb(port(Port::ModemControl), 0x1E);    // Set in loopback mode, test the serial chip
	outb(port(Port::Data), test_value_3);
	if (inb(port(Port::Data)) != test_value_3) {
		die("Serial console initialization failed");
	}
	outb(port(Port::ModemControl), 0x0F);

	setStyle(style);
}

bool SerialConsole::canTransmit()
{
	return inb(port(Port::LineStatus)) & 0x20;
}

void SerialConsole::writeChar(char ch)
{
	while (!canTransmit())
		delay();
	outb(port(Port::Data), ch);
}

void SerialConsole::writeLine()
{
	writeChar('\n');
}

void SerialConsole::writeValue(const char* string)
{
	for (; *string; string++)
		writeChar(*string);
}

void SerialConsole::setStyle(Style _style)
{
// See console_codes(4)
	static const char background_codes[16][4] = {
		"40", "44", "42", "46", "41", "45", "43", "47",
		"100", "104", "102", "106", "101", "105", "103", "107",
	};
	static const char foreground_codes[16][3] = {
		"30", "34", "32", "36", "31", "35", "33", "37",
		"90", "94", "92", "96", "91", "95", "93", "97",
	};
	style = _style;
	writeValue("\e[");
	writeValue(background_codes[style.background]);
	writeValue(";");
	writeValue(foreground_codes[style.foreground]);
	writeValue("m");
}

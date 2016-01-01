#include "console.hxx"

KConsole *KConsole::console = nullptr;
IConsole *kout;

// CDummyConsole

Style CDummyConsole::getStyle() const
{
	return{Color::LightGray, Color::Black};
}

void CDummyConsole::setStyle(Style style)
{
}

void CDummyConsole::writeChar(char ch)
{
}

void CDummyConsole::writeLine(const char* string)
{
}

void CDummyConsole::writeLine()
{
}

void CDummyConsole::writeValue(long int number, int base, int width)
{
}

void CDummyConsole::writeValue(const char* string)
{
}

// Console

Console::Console(ITerminal *_terminal):
	terminal(_terminal), style(Color::LightGray, Color::Black)
{
}

// Console::ITerminal

Symbol *Console::get()
{
	return terminal->get();
}

Symbol *Console::get(TermPos const& pos)
{
	return terminal->get(pos);
}

void Console::clear(char c)
{
	terminal->clear(c);
}

void Console::clearEx(Symbol s)
{
	terminal->clearEx(s);
}

void Console::clearLine(long y, char c)
{
	terminal->clearLine(y, c);
}

void Console::clearLineEx(long y, Symbol s)
{
	terminal->clearLineEx(y, s);
}

void Console::scrollVertically(long lines)
{
	terminal->scrollVertically(lines);
}

void Console::skipChar()
{
	terminal->skipChar();
}

void Console::skipLine()
{
	terminal->skipLine();
}

void Console::putChar(char c)
{
	terminal->putChar(c);
}

void Console::putCharEx(Symbol s)
{
	terminal->putCharEx(s);
}

void Console::putString(char const *line)
{
	terminal->putString(line);
}

void Console::putStringEx(char const *line, u1 style)
{
	terminal->putStringEx(line, style);
}

long Console::getWidth() const
{
	return terminal->getWidth();
}

long Console::getHeight() const
{
	return terminal->getHeight();
}

TermPos const& Console::getCursorPos() const
{
	return terminal->getCursorPos();
}

void Console::setCursorPos(TermPos const& pos)
{
	terminal->setCursorPos(pos);
}

// Console::IConsole

Style Console::getStyle() const
{
	return style;
}

void Console::setStyle(Style s)
{
	style = s;
}

void Console::writeChar(char ch)
{
	putCharEx(ch, style);
}

void Console::writeValue(char const *string)
{
	putStringEx(string, style);
}

void Console::writeValue(long number, int base, int full_width)
{
	static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if((base < 2) || (base >= static_cast<int>(sizeof(digits))))
	{
		writeValue("<base is not correct: ");
		writeValue(base, 10);
		writeValue(">");
		return;
	}
	/*if(!number)
	{
		writeChar('0');
		return;
	}*/
	if(number < 0)
	{
		writeChar('-');
		number = -number;
		--full_width;
	}
	long temp{number};
	long factor{1};
	int width{1};
	while(temp >= base)
	{
		temp /= base;
		factor *= base;
		++width;
	}
	for(; full_width > width; --full_width)
	{
		writeChar('0');
	}
	while(width)
	{
		int digit = number / factor;
		number %= factor;
		factor /= base;
		writeChar(digits[digit]);
		--width;
	}
}

void Console::writeLine()
{
	skipLine();
}

void Console::writeLine(char const *string)
{
	writeValue(string);
	writeLine();
}

#include "console.hxx"

KConsole *KConsole::console = nullptr;

// Console

Console::Console(ITerminal *_terminal):
	terminal(_terminal), style()
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

void Console::clearLine(long y, char c)
{
	terminal->clearLine(y, c);
}

void Console::clearLineEx(long y, char c, u1 style)
{
	terminal->clearLineEx(y, c, style);
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

void Console::putCharEx(char c, u1 style)
{
	terminal->putCharEx(c, style);
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

void Console::write(char ch)
{
	putCharEx(ch, style);
}

void Console::write(char const *string)
{
	putStringEx(string, style);
}

void Console::writeLine()
{
	skipLine();
}

void Console::writeLine(char const *string)
{
	write(string);
	writeLine();
}

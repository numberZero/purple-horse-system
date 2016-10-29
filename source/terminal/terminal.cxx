#include "terminal.hxx"
#include "memory.h"
#include "hardware/port.h"

void *const KTerminal::Base = (void *)0x000B8000;
KTerminal *KTerminal::terminal = nullptr;

MemMappedTerminal::MemMappedTerminal(void *_base, long _width, long _height, long _pitch):
	data(reinterpret_cast<Symbol *>(_base)), width(_width), height(_height), pitch(_pitch ? _pitch : _width),
	cursor(0, 0)
{
}

Symbol *MemMappedTerminal::get()
{
	return get(cursor);
}

Symbol *MemMappedTerminal::get(TermPos const& pos)
{
	if(pos.x < 0)
		return nullptr;

	if(pos.y < 0)
		return nullptr;

	if(pos.x >= width)
		return nullptr;

	if(pos.y >= height)
		return nullptr;

	return data + pos.y * pitch + pos.x;
}

long MemMappedTerminal::getWidth() const
{
	return width;
}

long MemMappedTerminal::getHeight() const
{
	return height;
}

TermPos const& MemMappedTerminal::getCursorPos() const
{
	return cursor;
}

void MemMappedTerminal::setCursorPos(TermPos const& pos)
{
	cursor.x = pos.x < 0 ? 0 : (pos.x >= width ? width - 1 : pos.x);
	cursor.y = pos.y < 0 ? 0 : (pos.y >= height ? height - 1 : pos.y);
}

void MemMappedTerminal::scrollVertically(long lines)
{
	if(!lines)
		return;
	if(pitch == width)
	{
		if(lines > 0)
			memmove(getLine(0), getLine(lines), width * (height - lines) * sizeof(Symbol));
		else
			memmove(getLine(-lines), getLine(0), width * (height + lines) * sizeof(Symbol));
		return;
	}
	if(lines > 0) // scrolling up
	{
		for(long dest = 0; dest < height - lines; ++dest)
			memcpy(getLine(dest), getLine(dest + lines), sizeof(Symbol) * width);
	}
	else // scrolling down
	{
		for(long dest = height - 1; dest >= -lines; --dest)
			memcpy(getLine(dest), getLine(dest + lines), sizeof(Symbol) * width);
	}
}

void MemMappedTerminal::clear(char c)
{
	for(long y = 0; y < height; ++y)
		for(long x = 0; x < width; ++x)
			get(TermPos(x, y))->symbol = c;
}

void MemMappedTerminal::clearEx(Symbol s)
{
	if(pitch == width)
		memsetw(data, s, width * height);
	else
		for(long y = 0; y < height; ++y)
			memsetw(getLine(y), s, width);
}

void MemMappedTerminal::clearLine(long y, char c)
{
	if(y < 0)
		return;
	if(y >= height)
		return;
	for(long x = 0; x < width; ++x)
		get(TermPos(x, y))->symbol = c;
}

void MemMappedTerminal::clearLineEx(long y, Symbol s)
{
	if(y < 0)
		return;
	if(y >= height)
		return;
	memsetw(getLine(y), s, width);
}

void MemMappedTerminal::skipChar()
{
	TermPos pos(cursor);
	++pos.x;
	if(pos.x >= width)
		skipLine();
	else
		setCursorPos(pos);
}

void MemMappedTerminal::skipLine()
{
	TermPos pos(cursor);
	pos.x = 0;
	++pos.y;
	if(pos.y >= height)
	{
		scrollVertically(pos.y - height + 1);
		pos.y = height - 1;
		clearLine(pos.y, 0);
	}
	setCursorPos(pos);
}

void MemMappedTerminal::putChar(char c)
{
	get()->symbol = c;
	skipChar();
}

void MemMappedTerminal::putCharEx(Symbol s)
{
	*get() = s;
	skipChar();
}

void MemMappedTerminal::putString(char const *line)
{
	while(*line)
		putChar(*(line++));
}

void MemMappedTerminal::putStringEx(char const *line, u1 style)
{
	while(*line)
		putCharEx(*(line++), style);
}

KTerminal::KTerminal(): MemMappedTerminal(Base, Width, Height)
{
}

void KTerminal::setCursorPos(TermPos const& pos)
{
	MemMappedTerminal::setCursorPos(pos);
	TermPos p(getCursorPos());
	u2 l = p.y * getWidth() + p.x;
	outb(0x3D4, 14);
	outb(0x3D5, l >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, l);
}

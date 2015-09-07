#pragma once
#include "types.h"

union __attribute__((packed)) Symbol
{
	u2 value;
	struct
	{
		char symbol;
		u1 style;
	};
	
	explicit Symbol(u2 _value): value(_value) {}
	explicit Symbol(char _symbol = ' ', u1 _style = 0x70): symbol(_symbol), style(_style) {}
};

struct TermPos
{
	long x;
	long y;
	
	TermPos(long _x, long _y): x(_x), y(_y) {}
};

class ITerminal
{
public:
	virtual Symbol *get() = 0;
	virtual Symbol *get(TermPos const& pos) = 0;
	
	virtual void clearLine(long y, char c) = 0;
	virtual void clearLineEx(long y, char c, u1 style) = 0;
	virtual void scrollVertically(long lines) = 0;
	
	virtual void skipChar() = 0;
	virtual void skipLine() = 0;
	virtual void putChar(char c) = 0;
	virtual void putCharEx(char c, u1 style) = 0;
	virtual void putString(char const *line) = 0;
	virtual void putStringEx(char const *line, u1 style) = 0;
	
	virtual long getWidth() const = 0;
	virtual long getHeight() const = 0;
	virtual TermPos const& getCursorPos() const = 0;
	virtual void setCursorPos(TermPos const& pos) = 0;
	
	inline long getCursorX() const;
	inline long getCursorY() const;
	inline long setCursorX(long x);
	inline long setCursoY(long y);
};

class MemMappedTerminal: virtual public ITerminal
{
private:
	Symbol *data;
	long width;
	long height;
	long pitch;
	TermPos cursor;
	
	inline Symbol *getLine(long y);
	
public:
	MemMappedTerminal(void *_base, long _width, long _height, long _pitch = 0);
	Symbol *get() override;
	Symbol *get(TermPos const& pos) override;
	
	void clearLine(long y, char c) override;
	void clearLineEx(long y, char c, u1 style) override;
	void scrollVertically(long lines) override;
	
	void skipChar() override;
	void skipLine() override;
	void putChar(char c) override;
	void putCharEx(char c, u1 style) override;
	void putString(char const *line) override;
	void putStringEx(char const *line, u1 style) override;
	
	long getWidth() const override;
	long getHeight() const override;
	TermPos const& getCursorPos() const override;
	void setCursorPos(TermPos const& pos) override;
};

class KTerminal: public MemMappedTerminal
{
public:
	static KTerminal *terminal;
	static void *const Base;
	static const long Width = 80;
	static const long Height = 25;
	
	KTerminal();
	void setCursorPos(TermPos const& pos);
};

Symbol *MemMappedTerminal::getLine(long y)
{
	return data + y * pitch;
}

long ITerminal::getCursorX() const
{
	return getCursorPos().x;
}

long ITerminal::getCursorY() const
{
	return getCursorPos().y;
}

long ITerminal::setCursorX(long x)
{
	TermPos pos(getCursorPos());
	pos.x = x;
	setCursorPos(pos);
}

long ITerminal::setCursoY(long y)
{
	TermPos pos(getCursorPos());
	pos.y = y;
	setCursorPos(pos);
}

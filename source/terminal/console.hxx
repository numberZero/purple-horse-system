#pragma once
#include "terminal.hxx"

enum Color
{
	Black		= 0b0000,
	Red			= 0b0001,
	Green		= 0b0010,
	Brown		= 0b0011,
	Blue		= 0b0100,
	Purple		= 0b0101,
	Teal			= 0b0110,
	Gray		= 0b0111,
	Light		= 0b1000,
	LightRed	= 0b1001,
	LightGreen	= 0b1010,
	Yellow		= 0b1011,
	LightBlue	= 0b1100,
	Magenta	= 0b1101,
	Cyan		= 0b1110,
	White		= 0b1111,
	
	Dark		= Black,
	DarkGray	= Light + Black,
	LightGray	= Gray,
	Fuchsia		= Magenta
};

struct Style
{
	union
	{
		u1 style;
		struct
		{
			Color foreground:4;
			Color background:4;
		};	
	};
	
	Style(u1 s): style(s) {}
	Style(Color fg = LightGray, Color bg = Black): foreground(fg), background(bg) {}
	Style& operator= (u1 s) { style = s; }
	operator u1() { return style; }
} __attribute__((packed));

class IConsole
{
public:
	virtual Style getStyle() const = 0;
	virtual void setStyle(Style style) = 0;
};

class Console: virtual public IConsole, virtual public ITerminal
{
private:
	ITerminal *terminal;
	Style style;
	
public:
	Console(ITerminal *_terminal);
	
// ITerminal
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
	
// IConsole
	Style getStyle() const override;
	void setStyle(Style style) override;
	void write(char ch);
	void write(char const *string);
	void writeLine();
	void writeLine(char const *string);
};

class KConsole: public Console
{
public:
	static KConsole *console;
	
	KConsole():
		Console(KTerminal::terminal)
	{
	}
};

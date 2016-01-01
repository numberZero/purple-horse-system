#pragma once
#include "types.hxx"
#include "terminal.hxx"

class IConsole
{
public:
	virtual Style getStyle() const = 0;
	virtual void setStyle(Style style) = 0;
	virtual void writeChar(char ch) = 0;
	virtual void writeValue(char const *string) = 0;
	virtual void writeValue(long number, int base = 10, int width = 1) = 0;
	virtual void writeLine() = 0;
	virtual void writeLine(char const *string) = 0;
	
	void write(char const* string)
	{
		writeValue(string);
	}
	
	void write(long arg)
	{
		writeValue(arg);
	}
	
	void write(void const* arg)
	{
		writeValue("0x");
		writeValue(reinterpret_cast<u4>(arg), 16, 8);
	}
	
	template <typename T1, typename T2, typename... Args>
	void write(T1 arg1, T2 arg2, Args... args)
	{
		write(arg1);
		write(arg2, args...);
	}
	
	template <typename... Args>
	void writeLine(Args... args)
	{
		write(args...);
		writeLine();
	}
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
	
	void clear(char c) override;
	void clearEx(Symbol s) override;
	void clearLine(long y, char c) override;
	void clearLineEx(long y, Symbol s) override;
	void scrollVertically(long lines) override;
	
	void skipChar() override;
	void skipLine() override;
	void putChar(char c) override;
	void putCharEx(Symbol s) override;
	void putString(char const *line) override;
	void putStringEx(char const *line, u1 style) override;
	
	long getWidth() const override;
	long getHeight() const override;
	TermPos const& getCursorPos() const override;
	void setCursorPos(TermPos const& pos) override;
	
	using ITerminal::clearEx;
	using ITerminal::clearLineEx;
	using ITerminal::putCharEx;
	
// IConsole
	Style getStyle() const override;
	void setStyle(Style style) override;
	void writeChar(char ch) override;
	void writeValue(char const *string) override;
	void writeValue(long number, int base = 10, int width = 0) override;
	void writeLine() override;
	void writeLine(char const *string) override;
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

class CDummyConsole: virtual public IConsole
{
public:
	Style getStyle() const override;
	void setStyle(Style style) override;
	void writeChar(char ch) override;
	void writeValue(char const *string) override;
	void writeValue(long number, int base = 10, int width = 1) override;
	void writeLine() override;
	void writeLine(char const *string) override;
};

extern IConsole *kout;

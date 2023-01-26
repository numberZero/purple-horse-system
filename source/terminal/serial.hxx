#pragma once
#include "console.hxx"

class SerialConsole: virtual public IConsole
{
private:
	enum class Port;

	int base_port;
	Style style = {Gray, Black};

	int port(Port kind);
	bool canTransmit();

public:
	SerialConsole(int _base_port);

// IConsole
	Style getStyle() const override { return style; }
	void setStyle(Style style) override;
	void writeChar(char ch) override;
	void writeValue(char const *string) override;
	void writeLine() override;
};

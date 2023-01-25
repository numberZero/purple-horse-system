ENTRY(start)
/* STARTUP(boot.S.o) */
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
	.text 0x500 :
	{
		code = .; _code = .; __code = .;
		*(header)
		. = ALIGN(4096);
	}

	.stack 0x00010000 :
	{
		stack_segment = .; stack_l = .;
		. = 0x00070000;
		stack = .; stack_h = .;
	}

	.text 0x00100000 :
	{
		code = .; _code = .; __code = .;
		*(bootstrap)
		*(.text)
	}

	. = ALIGN(4096);
	.data :
	{
		data = .; _data = .; __data = .;
		*(.data)
		*(.rodata)
	}

	. = ALIGN(4096);
	.bss :
	{
		bss = .; _bss = .; __bss = .;
		*(.bss)
	}

	. = ALIGN(4096);
	.heap :
	{
		heap = .; _heap = .; __heap = .;
		. = . + 0x100000;
	}

	. = ALIGN(4096);
	end = .; _end = .; __end = .;
}

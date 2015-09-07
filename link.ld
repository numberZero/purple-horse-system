/* link.ld -- скрипт компоновщика ядра - проследите за тем, чтобы */
/*            это выполнялось в правильном месте.  */
/*            Исходный файл был взят из руководства по разработке ядра Bran's  */
/*            Kernel Development: http://www.osdever.net/bkerndev/index.php. */

ENTRY(start)
SECTIONS
{
	.header 0x200 :
	{
		*(header)
		. = ALIGN(4096);
	}

	.text 0x00100000 :
	{
/*		stack = .; _stack = .; __stack = .;*/
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
	}

	. = ALIGN(4096);
	end = .; _end = .; __end = .;
}

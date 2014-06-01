#include "defines.S"

FUNCTION(outb) # void outb(u2 port, u1 value)
	movw 8(%ebp), %dx # %dx = port
	movb 12(%ebp), %al # %al = value
	outb %al, %dx
ENDFUNCTION(outb)
	
FUNCTION(outw) # void outb(u2 port, u2 value)
	movw 8(%ebp), %dx # %dx = port
	movw 12(%ebp), %ax # %ax = value
	outw %ax, %dx
ENDFUNCTION(outb)
	
FUNCTION(inb) # u1 inb(u2 port)
	movw 8(%ebp), %dx # %dx = port
	inb %dx, %al
ENDFUNCTION(inb)

FUNCTION(inw) # u2 inw(u2 port)
	movw 8(%ebp), %dx # %dx = port
	inw %dx, %ax
ENDFUNCTION(inw)

.code32

.text
	.global gdt_flush
	gdt_flush:
		movl 4(%esp), %eax
		lgdt (%eax)
		
		movw $16, %ax
		movw %ax, %ds
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs
		movw %ax, %ss
		
		8:jmp .flush
	.flush:
		ret

.code32

.extern interrupt_handler

.text
	.global idt_flush
	idt_flush:
		movl 4(%esp), %eax
		lidt (%eax)
		ret

	.global interrupt_handler_wrapper
	interrupt_handler_wrapper:
		pushal

		xorl %eax, %eax
		movw %ds, %ax
		pushl %eax

		movw $16, %ax
		movw %ax, %ds
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs

		pushl %esp
		call interrupt_handler
		addl $4, %esp

		popl %eax
		movw %ax, %ds
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs

		popal
		addl $8, %esp
		iret

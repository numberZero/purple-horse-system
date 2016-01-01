.code32

.extern interrupt_handler

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
		
		movw %cs, %ax
		
		ljmp $8, $.flush
	.flush:
		ret
	
	.global idt_flush
	idt_flush:
		movl 4(%esp), %eax
		lidt (%eax)
		ret

	.global interrupt_handler_wrapper
	interrupt_handler_wrapper:
		pushal

		movw %ds, %ax
		pushl %eax

		movw $16, %ax
		movw %ax, %ds
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs

		call interrupt_handler

		popl %eax
		movw %ax, %ds
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs

		popal
		addl $8, %esp
		sti
		iret

	.global irq_handler_wrapper
	irq_handler_wrapper:
		pushal

		movw %ds, %ax
		pushl %eax

		movw $16, %ax
		movw %ax, %ds
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs

		call irq_handler

		popl %eax
		movw %ax, %ds
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs

		popal
		addl $8, %esp
		sti
		iret

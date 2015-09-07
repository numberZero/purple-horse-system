.code32

.extern interrupt_handler

.macro interrupt_handler_noerrcode num
	.global interrupt_handler_\num
	interrupt_handler_\num:
		cli
		pushl $0
		pushl $\num
		jmp interrupt_handler_wrapper
		nop
.endm

.macro interrupt_handler_witherrcode num
	.global interrupt_handler_\num
	interrupt_handler_\num:
		cli
		pushl $\num
		jmp interrupt_handler_wrapper
		nop
.endm

.macro irq_handler num
	.global irq_handler_\num
	irq_handler_\num:
		cli
		pushl $\num
		pushl $(32 + \num)
		jmp irq_handler_wrapper
		nop
.endm

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
		sti
		ret

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

# CPU interrupts
interrupt_handler_noerrcode	0	# Division by zero exception
interrupt_handler_noerrcode	1	# Debug exception
interrupt_handler_noerrcode	2	# Non maskable interrupt
interrupt_handler_noerrcode	3	# Breakpoint exception
interrupt_handler_noerrcode	4	# 'Into detected overflow'
interrupt_handler_noerrcode	5	# Out of bounds exception
interrupt_handler_noerrcode	6	# Invalid opcode exception
interrupt_handler_noerrcode	7	# No coprocessor exception
interrupt_handler_witherrcode	8	# Double fault (pushes an error code)
interrupt_handler_noerrcode	9	# Coprocessor segment overrun
interrupt_handler_witherrcode	10	# Bad TSS (pushes an error code)
interrupt_handler_witherrcode	11	# Segment not present (pushes an error code)
interrupt_handler_witherrcode	12	# Stack fault (pushes an error code)
interrupt_handler_witherrcode	13	# General protection fault (pushes an error code)
interrupt_handler_noerrcode	14	# Page fault (pushes an error code)
interrupt_handler_noerrcode	15	# Unknown interrupt exception
interrupt_handler_noerrcode	16	# Coprocessor fault
interrupt_handler_noerrcode	17	# Alignment check exception
interrupt_handler_noerrcode	18	# Machine check exception
interrupt_handler_noerrcode	19	# RESERVED, so next are
interrupt_handler_noerrcode	20
interrupt_handler_noerrcode	21
interrupt_handler_noerrcode	22
interrupt_handler_noerrcode	23
interrupt_handler_noerrcode	24
interrupt_handler_noerrcode	25
interrupt_handler_noerrcode	26
interrupt_handler_noerrcode	27
interrupt_handler_noerrcode	28
interrupt_handler_noerrcode	29
interrupt_handler_noerrcode	30
interrupt_handler_noerrcode	31

# IRQs
irq_handler	0
irq_handler	1
irq_handler	2
irq_handler	3
irq_handler	4
irq_handler	5
irq_handler	6
irq_handler	7
irq_handler	8
irq_handler	9
irq_handler	10
irq_handler	11
irq_handler	12
irq_handler	13
irq_handler	14
irq_handler	15

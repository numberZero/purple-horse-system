.code32

.text
	.global set_context
	set_context:
		pushl %ebp
		movl %esp, %ebp
		movl 8(%ebp), %edx # SInterruptRegisters *
		movl 12(%ebp), %eax # SCommonRegisters *

		movl 12(%eax), %esp # Restore ESP to saved value
		# It was saved *before* change, so points right on the place where SInterruptRegisters should be copied
		movl %edx, %esi
		movl %esp, %edi
		movl $3, %ecx
		rep movsl

		# Now it’s time to restore registers
		movl (%eax), %edi
		movl 4(%eax), %esi
		movl 8(%eax), %ebp
		movl 12(%eax), %esp
		movl 16(%eax), %ebx
		movl 20(%eax), %edx
		movl 24(%eax), %ecx
		movl 28(%eax), %eax

		iret

	.global task_wrapper
	task_wrapper:
		pushl %eax
		pushl %edx
		call task_wrapper_2
		jmp halt

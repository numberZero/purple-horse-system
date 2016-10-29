.set MBOOT_PAGE_ALIGN, 1<<0
.set MBOOT_MEM_INFO, 1<<1
.set MBOOT_HEADER_MAGIC, 0x1BADB002
.set MBOOT_HEADER_FLAGS, MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
.set MBOOT_CHECKSUM, -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

.code32

.section header
	.extern code
	.extern data
	.extern stack
	.extern end

	mboot:
		.long  MBOOT_HEADER_MAGIC
		.long  MBOOT_HEADER_FLAGS
		.long  MBOOT_CHECKSUM

		.long  mboot
		.long  code
		.long  data
		.long  end
		.long  start

.section bootstrap
	.global start
	.global halt
	.extern kernel_main

	start:
		cli
#ifdef VIDEO_STACK
		movl $(0x000B8000 + 80*25*2), %esp
		pushl $0x0C5A0C5A
		pushl $0xCF5ACF5A
#else
		movl $stack, %esp
#endif
		pushl %ebx
		call kernel_main

	halt:
		cli
		hlt
		jmp halt

.text
	.global getStackPointer
	getStackPointer:
		movl %esp, %eax
		ret

	.global getCodePointer
	getCodePointer:
		movl (%esp), %eax
		ret

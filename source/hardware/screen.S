.code32
.data
	screen_base: .long 0x000B8000
	screen_sx: .long 80
	screen_sy: .long 24
	cursor_x: .long 0
	cursor_y: .long 0

.text
	.global clearscreen
	clearscreen:
		pushl %edi
		movl $0x07200720, %eax
		movl screen_base, %edi
		movl screen_sx, %ecx
		imull screen_sy, %ecx
		shrl $1, %ecx
		rep stosl
		popl %edi
		ret

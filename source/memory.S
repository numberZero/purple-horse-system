.code32
.text

#include "defines.S"

ALIAS(memset, memsetb) # void *memset(void *s, int c, size_t n);

FUNCTION(memsetb) # void *memsetb(void *s, int c, size_t n);
	pushl %edi
	movl 8(%ebp), %edi # %edi = s
	movl 12(%ebp), %eax # %eax = c
	movb %al, %ah
	movl %eax, %ecx
	shll $16, %eax
	movw %cx, %ax
	movl 16(%ebp), %ecx # %ecx = n
	shrl $2, %ecx
	cld
	rep stosl
	movl 16(%ebp), %ecx # %ecx = n
	andl $3, %ecx
	rep; stosb
	movl 8(%ebp), %eax # %eax = s
	popl %edi
ENDFUNCTION(memsetb)

FUNCTION(memsetw) # void *memsetw(void *s, int c, size_t n);
	pushl %edi
	movl 8(%ebp), %edi # %edi = s
	movl 12(%ebp), %eax # %eax = c
	movl %eax, %ecx
	shll $16, %eax
	movw %cx, %ax
	movl 16(%ebp), %ecx # %ecx = n
	shrl $1, %ecx
	cld
	rep stosl
	movl 16(%ebp), %ecx # %ecx = n
	andl $1, %ecx
	jz skip1
	stosw
skip1:
	movl 8(%ebp), %eax # %eax = s
	popl %edi
ENDFUNCTION(memsetw)

FUNCTION(memsetd) # void *memsetd(void *s, int c, size_t n);
	pushl %edi
	movl 8(%ebp), %edi # %edi = s
	movl 12(%ebp), %eax # %eax = c
	movl 16(%ebp), %ecx # %ecx = n
	cld
	rep stosl
	movl 8(%ebp), %eax # %eax = s
	popl %edi
ENDFUNCTION(memsetd)

ALIAS(memcpy, memmove)
/*
FUNCTION(memcpy) # void *memcpy(void *dest, const void *src, size_t n);
	pushl %esi
	pushl %edi
	movl 8(%ebp), %edi # %edi = s
	movl 12(%ebp), %esi # %eax = c
	movl 16(%ebp), %ecx # %eax = c
	
	popl %edi
	popl %esi
ENDFUNCTION(memcpy)
*/

FUNCTION(memmove) # void *memmove(void *dest, const void *src, size_t n);
	pushl %esi
	pushl %edi
	movl 8(%ebp), %edi # %edi = dest
	movl 12(%ebp), %esi # %esi = src
	movl 16(%ebp), %ecx # %ecx = n
	cmpl %esi, %edi
	jns reverse
	cld # normal direction
	rep movsb
	jmp exit
reverse:
	std # reverse direction
	lea -1(%esi, %ecx), %esi
	lea -1(%edi, %ecx), %edi
	rep movsb
exit:
	movl 8(%ebp), %eax # %eax = dest
	popl %edi
	popl %esi
ENDFUNCTION(memmove)

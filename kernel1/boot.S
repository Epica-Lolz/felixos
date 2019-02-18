.set ALIGN,	1<<0
.set MEMMAP,	1<<1

.set FLAGS, 		ALIGN | MEMMAP
.set MAGIC_NUMBER,	0x1BADB002
.set CHECKSUM,		-(MAGIC_NUMBER + FLAGS)

.section .multiboot
.align 4
.long MAGIC_NUMBER
.long FLAGS
.long CHECKSUM

.set STACK_SIZE, 16384

.section .data
.align 16
stack:
	.lcomm buffer, STACK_SIZE

.section .text
.global _start
.type _start, @function

_start:
	mov $(stack + STACK_SIZE), %esp
	call k_main
	
	cli
1:	hlt
	jmp 1b

.size _start, . - _start

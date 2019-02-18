.set ALIGN,	1<<0				/*align loaded modules on page boundaries*/
.set MEMMAP,	1<<1				/*provide memory map*/

.set FLAGS, 		ALIGN | MEMMAP		/*bit 0 of FLAGS is set to 1, and bit 1 of flags is set to 1*/
.set MAGIC_NUMBER,	0x1BADB002		/*GRUB magic number*/
.set CHECKSUM,		-(MAGIC_NUMBER + FLAGS)	/*checksum*/

/*Multiboot section (so that grub sees that this is a kernel)*/ 
.section .multiboot
.align 4		/*grub will search for these values in the first 8 KiB of the kernel file, which is aligned at a 32-bit (4-byte) boundary*/
.long MAGIC_NUMBER
.long FLAGS
.long CHECKSUM

.set STACK_SIZE, 16384	/*16KB*/

/*reserve 16 KB for stack (Necessary for C, since it will not function without a stack)*/
.section .data
.align 16
stack:
	.lcomm buffer, STACK_SIZE

.section .text
.global _start
.type _start, @function

_start:
	/*move the top of the stack to esp*/
	mov $(stack + STACK_SIZE), %esp
	
	/*call kernel main function*/
	call k_main
	
	/*clear all interrupts, technically not needed since interrupts are already disabled by the bootloader*/
	cli
1:	hlt	/*halt instruction*/
	jmp 1b	/*infinite loop*/

/*Set the size of the _start symbol to the current location ('.') minus its start. Useful for debugging.*/
.size _start, . - _start

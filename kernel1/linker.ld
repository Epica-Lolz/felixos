ENTRY (_start)

SECTIONS {
	. = 0x00100000;
	
	.text BLOCK(0x1000) : ALIGN(0x1000)
	{
		*(.multiboot)
		*(.text)
	}
	.rodata BLOCK(0x1000) : ALIGN(0x1000)
	{
		*(.rodata*)
	}
	.data BLOCK(0x1000) : ALIGN(0x1000)
	{
		*(.data)
	}
	.bss BLOCK(0x1000) : ALIGN(0x1000)
	{
		*(COMMON)
		*(.bss)
	}
}

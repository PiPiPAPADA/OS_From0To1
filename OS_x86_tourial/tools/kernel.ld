/*link script*/

ENTRY(start)
SECTIONS
{
	PROVIDE( kern_start = 0xC0100000 );
	. = 0x100000;
    .init_text :
    {
        *(.init_text)
        . = ALIGN(4096);
    }
    .init_data :
    {
        *(.init_data)
        . = ALIGN(4096);
    }
    . += 0xC0000000;
	.text : AT(ADDR(.text) - 0xC0000000)
	{
		*(.text)
		. = ALIGN(4096);
	}
	.data : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}
	.bss : AT(ADDR(.bss) - 0xC0000000)
	{
		*(.bss)
		. = ALIGN(4096);
	}
	.stab : AT(ADDR(.stab) - 0xC0000000)
	{
		*(.stab)
		. = ALIGN(4096);
	}
	.stabstr : AT(ADDR(.stabstr) - 0xC0000000)
	{
		*(.stabstr)
	 	. = ALIGN(4096);
	}
	PROVIDE( kern_end = . );

	/DISCARD/ : { *(.comment) *(.eh_frame) }
}
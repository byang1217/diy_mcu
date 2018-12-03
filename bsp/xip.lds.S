#define __ASSEMBLY__
#include "bsp.h"

MEMORY
{
    ROM (rx) : ORIGIN = ROM_START, LENGTH = ROM_LEN
    RAM (rw) : ORIGIN = RAM_START, LENGTH = RAM_LEN
}

SECTIONS
{
	.isr_vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector))
		. = ALIGN(4);
	} >ROM

	.text :
	{
		. = ALIGN(4);
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata*)
		. = ALIGN(4);
		_etext = .;
		_sidata = _etext;
	} >ROM

	.data  : AT ( _sidata )
	{
		. = ALIGN(4);
		_sdata = . ;
		*(.data)
		*(.data.*)
		. = ALIGN(4);
		_edata = . ;
	} >RAM

	.bss :
	{
	    . = ALIGN(4);
        _sbss = .;
		*(.bss)
		*(.bss.*)
		*(COMMON)
	    . = ALIGN(4);
   	 _ebss = . ;
	} > RAM

	end = _ebss;
	_end = _ebss;

	_estack = RAM_START + RAM_LEN - 4;
}

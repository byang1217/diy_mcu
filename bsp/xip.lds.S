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

		. = ALIGN(4);
		*(.glue_7)         /* glue arm to thumb code */
		*(.glue_7t)        /* glue thumb to arm code */
		*(.eh_frame)

		. = ALIGN(4);
		KEEP (*(.init))
		KEEP (*(.fini))
		. = ALIGN(4);
	} >ROM

	.rodata :
	{
		. = ALIGN(4);
		*(.rodata)
		*(.rodata*)
		. = ALIGN(4);
	} >ROM

	.ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >ROM
	.ARM : {
		__exidx_start = .;
		*(.ARM.exidx*)
		__exidx_end = .;
	} >ROM

	.preinit_array     :
	{
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array*))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	} >ROM
	.init_array :
	{
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		PROVIDE_HIDDEN (__init_array_end = .);
	} >ROM
	.fini_array :
	{
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array*))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} >ROM

	.stack :
	{
		. = ALIGN(4);
		_sstack = .;
		. += STACK_SIZE;
		. = ALIGN(4);
		_estack = . ;
	} > RAM

	/* used by the startup to initialize data */
	_sidata = LOADADDR(.data);
	.data :
	{
		. = ALIGN(4);
		_sdata = . ;
		*(.data)
		*(.data.*)
		. = ALIGN(4);
		_edata = . ;
	} >RAM AT>ROM

	. = ALIGN(4);
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

	/* Remove information from the standard libraries */
	/DISCARD/ :
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}
}

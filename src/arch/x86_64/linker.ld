ENTRY(_start)

SECTIONS {
	. = 1M;

	.boot : {
		KEEP(*(.multiboot_header))
	}

	.text : {
		*(.text)
		*(.rodata*)
		. = ALIGN(4096);
	}

	.data : {
		*(.data)
		. = ALIGN(4096);
	}

	.eh_frame : {
       *(.eh_frame)
        . = ALIGN(4096);
	}

	.bss : {
		*(.bss)
		*(COMMON)
		. = ALIGN(4096);
	}
}

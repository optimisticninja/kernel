ENTRY(start)

SECTIONS {
	. = 1M;

	.boot : {
		KEEP(*(.multiboot_header))	/* Keep protects from optimization */
	}

	.text : {
		*(.text)
	}
}

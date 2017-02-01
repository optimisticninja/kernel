ENTRY(start)

SECTIONS {
	. = 1M;

	.boot : {
		KEEP(*(.multiboot_header))	/* KEEP - protects from optimization */
	}

	.text : {
		*(.text)
	}
}

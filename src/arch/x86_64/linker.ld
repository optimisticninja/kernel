ENTRY(start)

SECTIONS {
	. = VGA_MEMORY;

	.boot : {
		*(.multiboot_header)
	}

	.text : {
		*(.text)
	}
}

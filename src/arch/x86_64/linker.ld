ENTRY(start)

SECTIONS {
	. = 1M;

	.boot : {
		*(.multiboot_header)	/* KEEP ensures multiboot is at the beginning */
	}

	.text : {
		*(.text)
	}
}

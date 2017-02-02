ENTRY(start)

SECTIONS {
	. = 1M;

	.boot : {
		KEEP(*(.multiboot_header))	/* KEEP - protects from optimization */
	}

	.text : {
		*(.text .text.*)
	}

	.rodata : {
		*(.rodata .rodata.*)
	}

	.data.rel.ro : {
		*(.data.rel.ro.local*) *(.data.rel.ro .data.rel.ro.*)
	}
}

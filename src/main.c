#include <stdio.h>

#include <kernel/tty.h>

// TODO: Define multiboot struct and verify it is correct
typedef struct MultibootHeader MultibootHeader;

void kernel_early()
{
	terminal_initialize();
	terminal_setcolor(get_color_code(COLOR_BLACK, COLOR_YELLOW));
}

int kmain(__attribute__((__unused__)) MultibootHeader* mboot_hdr) 
{
	kernel_early();
	printf("[ KERNEL V0.0.1 (C) 2016 - John Holly ]\n\n");

	printf("Hello, world!");

	while (1) {

	}

	return 666;
}

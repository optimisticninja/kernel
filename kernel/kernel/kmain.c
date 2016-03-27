typedef struct Multiboot Multiboot;

#include <stdio.h>
#include <kernel/tty.h>

void kernel_early()
{
	terminal_initialize();
}

int kmain(__attribute__((__unused__)) Multiboot* mboot_ptr)
{
	printf("CRANK KERNEL v1.0 - (C) 2015, John Holly\n");

	return 0xDEADBABA;
}

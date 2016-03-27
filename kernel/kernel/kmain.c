typedef struct Multiboot Multiboot;

#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/descriptor_tables.h>

void kernel_early()
{
	terminal_initialize();
	init_descriptor_tables();
}

int kmain(__attribute__((__unused__)) Multiboot* mboot_ptr)
{
	printf("CRANK KERNEL v1.0 (C) 2015, John Holly\n");
	asm volatile ("int $0x3");
	asm volatile ("int $0x4");
	return 0xDEADBABA;
}

#include <stdio.h>
#include <stdint.h>

#include <kernel/kernel.h>
#include <kernel/timer.h>
#include <kernel/tty.h>
#include <kernel/descriptor_tables.h>
#include <kernel/paging.h>

typedef struct Multiboot Multiboot;

void kernel_early()
{
	terminal_initialize();
	terminal_clear();
	init_descriptor_tables();
}

int kmain(__attribute__((__unused__)) Multiboot* mboot_ptr)
{
	printf("CRANK KERNEL v1.0 (C) 2015, John Holly\n\n");
	init_paging();
	
	printf("Interrupts test.\n");

	asm volatile ("int $0x3");
	asm volatile ("int $0x4");

	printf("Paging test\n");

	uint32_t *ptr = (uint32_t*)0xA0000000;
	__attribute__((__unused__)) uint32_t do_page_fault = *ptr;
	//asm volatile ("sti");
	//init_timer(50);
	
	return 0xDEADBABA;
}

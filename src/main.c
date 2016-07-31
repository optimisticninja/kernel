#include <stdio.h>

#include <kernel/tty.h>

typedef struct MultibootHeader MultibootHeader;

void kernel_early()
{
	terminal_initialize();
}

int kmain(__attribute__((__unused__)) MultibootHeader* mboot_hdr) 
{
	kernel_early();
	printf("[ KERNEL V0.0.1 (C) 2016 - John Holly, Ryan Daniels, David Edwards ]\n");
	return 666;
}

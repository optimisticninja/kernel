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
	printf("Hello, world");
	return 666;
}

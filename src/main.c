#include <stdio.h>

#include <kernel/tty.h>
//#include <kernel/arch/x86_64/cpuid.h>

void kernel_early()
{
	terminal_initialize();
	terminal_setcolor(get_color_code(COLOR_BLACK, COLOR_YELLOW));

	
}

int kmain() 
{
	kernel_early();
	printf("[ KERNEL V0.0.1 (C) 2016 - John Holly ]\n\n");

	printf("Hello, world!\n%s", "String interpolation test");

	while (1) {

	}

	return 666;
}

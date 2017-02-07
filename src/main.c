#include <stdio.h>
#include <stdint.h>

#include <grub/multiboot2.h>
#include <kernel/tty.h>
#include <kernel/arch/x86_64/cpuid_info.h>

void kernel_early(struct multiboot_header* multiboot)
{
	terminal_initialize();
	terminal_setcolor(get_color_code(COLOR_BLACK, COLOR_YELLOW));	
	printf("[ KERNEL V0.0.1 (C) 2016 - John Holly ]\n\n");
	
	printf("\n==== MULTIBOOT ====\n");
	printf("Architecture:\t%s\n", (multiboot->architecture) ? "MIPS" : "i386");
	printf("Checksum:\t%u\n", multiboot->checksum);
	printf("Size:\t\t%u\n\n", multiboot->header_length);
	
	printf("\n==== CPU FEATURES ===\n");
	cpu_detect();	// Print CPU and Features
}

int kmain(struct multiboot_header* multiboot)
{
	kernel_early(multiboot);
	
	while (1);

	return 666;
}


#include <stdio.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/arch/x86_64/cpuid_info.h>

typedef struct {
	uint32_t type;
	uint32_t size;
} MultibootTag;

typedef struct {
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
	uint32_t reserved;
} MemoryArea;

typedef struct {
	uint32_t type;
	uint32_t size;
	uint32_t entry_size;
	uint32_t entry_version;
	MemoryArea* first_area;
} MultibootMemoryMapTag;

typedef struct {
	uint32_t total_size;
	uint32_t reserved;
	uint32_t first_tag;
} MultibootInfo;


void kernel_early(__attribute__((unused)) MultibootInfo* multiboot)
{
	terminal_initialize();
	terminal_setcolor(get_color_code(COLOR_BLACK, COLOR_YELLOW));	
	printf("[ KERNEL V0.0.1 (C) 2016 - John Holly ]\n\n");
	cpu_detect();	// Print CPU and Features
}

int kmain(MultibootInfo* multiboot)
{
	kernel_early(multiboot);	

	while (1);

	return 666;
}


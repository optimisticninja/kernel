#include <kernel/kernel.h>

#include <stdint.h>

#include <kernel/tty.h>

extern void panic(const char* msg, const char* file, uint32_t line)
{
	asm volatile("cli");

	terminal_writestr("PANIC(");	
	terminal_writestr(msg);
	terminal_writestr(") at ");
	terminal_writestr(file);
	terminal_putchar(':');
	terminal_write_dec(line);
	terminal_putchar('\n');

	while (1);
}

extern void panic_assert(const char* file, uint32_t line, const char* desc)
{
	asm volatile("cli");

	terminal_writestr("ASSERTION-FAILED(");	
	terminal_writestr(desc);
	terminal_writestr(") at ");
	terminal_writestr(file);
	terminal_putchar(':');
	terminal_write_dec(line);
	terminal_putchar('\n');

	while (1);
}

extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys)
{
    if (align == 1 && (placement_address & 0xFFFFF000)) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    } if (phys) {
        *phys = placement_address;
    }
    uint32_t tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint32_t kmalloc_a(uint32_t sz)
{
    return kmalloc_int(sz, 1, 0);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 0, phys);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 1, phys);
}

uint32_t kmalloc(uint32_t sz)
{
    return kmalloc_int(sz, 0, 0);
}


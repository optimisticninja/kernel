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

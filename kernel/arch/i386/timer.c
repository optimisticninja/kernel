#include <kernel/timer.h>

#include <stdint.h>

#include <kernel/io.h>
#include <kernel/isr.h>
#include <kernel/tty.h>

uint32_t tick = 0;

static void timer_callback(__attribute__((__unused__)) Registers regs)
{
	tick++;
	terminal_writestr("Tick: ");
	terminal_write_dec(tick);
	terminal_putchar('\n');
}

void init_timer(uint32_t frequency)
{
	register_interrupt_handler(IRQ0, &timer_callback);

	// 119280 Hz
	uint32_t divisor = 1193180 / frequency;

	// Command byte
	outb(0x43, 0x36);

	// Split into upper and lower
	uint8_t l = (uint8_t) (divisor & 0xFF);
	uint8_t h = (uint8_t) ((divisor >> 8) & 0xFF);

	// Send freq. divisor
	outb(0x40, l);
	outb(0x40, h);
}

#include <kernel/io.h>
#include <kernel/isr.h>

ISR interrupt_handlers[256];

void irq_handler(Registers regs)
{
	if (regs.int_no >= 40)
		outb(0xA0, 0x20);

	outb(0x20, 0x20);

	if (interrupt_handlers[regs.int_no] != 0) {
		ISR handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}

void register_interrupt_handler(uint8_t n, ISR handler)
{
	interrupt_handlers[n] = handler;
}

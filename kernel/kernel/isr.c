#include <kernel/isr.h>

#include <kernel/io.h>
#include <kernel/tty.h>

ISR interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, ISR handler)
{
    interrupt_handlers[n] = handler;
}

void isr_handler(Registers regs)
{
	if (interrupt_handlers[regs.int_no] != 0) {
        ISR handler = interrupt_handlers[regs.int_no];
        handler(regs);
    } else {
        terminal_writestr("unhandled interrupt: ");
        terminal_write_dec(regs.int_no);
        terminal_putchar('\n');
    }
}

void irq_handler(Registers regs)
{
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.int_no >= 40)
    {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
    {
        ISR handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }

}


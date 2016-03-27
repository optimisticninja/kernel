#include <kernel/isr.h>
#include <kernel/tty.h>

void isr_handler(Registers regs)
{
	terminal_writestr("recieved interrupt: ");
	terminal_write_dec(regs.int_no);
	terminal_putchar('\n');
}

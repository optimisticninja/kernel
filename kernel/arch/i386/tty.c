#include <kernel/tty.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <kernel/io.h>
#include <kernel/vga.h>

#define TO_HEX(i) (i <= 9 ? '0' + i : 'A' - 10 + i)

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_BLACK, COLOR_YELLOW);
	terminal_buffer = VGA_MEM_START;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

static void move_cursor()
{
	// The screen is 80 characters wide...
	uint16_t cursor_loc = terminal_row * 80 + terminal_column;
	outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
	outb(0x3D5, cursor_loc >> 8); // Send the high cursor byte.
	outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
	outb(0x3D5, cursor_loc);      // Send the low cursor byte.
}

static void scroll()
{
	uint16_t blank = ' ' | (terminal_color << 8);

	if (terminal_row >= 25) {
		for (unsigned i = 0 * VGA_WIDTH; i < (VGA_HEIGHT-1) * VGA_WIDTH; i++)
			terminal_buffer[i] = terminal_buffer[i + VGA_WIDTH];

		for (unsigned i = (VGA_HEIGHT - 1) * 80; i < VGA_HEIGHT * VGA_WIDTH; i++)
			terminal_buffer[i] = blank;

		terminal_row = 24;
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putchar(char c)
{
	uint16_t* location;

	if (c == '\b' && terminal_column) {
		terminal_column--;
	} else if (c == '\t') {
		terminal_column = (terminal_column + 8) & ~(8 - 1);
	} else if (c == '\r') {
		terminal_column = 0;
	} else if (c == '\n') {
		terminal_column = 0;
		terminal_row++;
	} else if (c >= ' ') {	// All other printables
		location = terminal_buffer + (terminal_row * 80 + terminal_column);
		*location = make_vgaentry(c, terminal_color);
		terminal_column++;
	}

	if (terminal_column >= VGA_WIDTH) {
		terminal_column = 0;
		terminal_row++;
	}

	scroll();
	move_cursor();
}

void terminal_clear()
{
	uint16_t blank = ' ' | (terminal_color << 8);

	for (unsigned i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++)
		terminal_buffer[i] = blank;

	terminal_column = 0;
	terminal_row = 0;
	move_cursor();
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_write_hex(uint32_t n)
{
	char str[9];

	if (n <= 0xFFFFFFFF) {
		str[0] = TO_HEX(((n & 0xF0000000) >> 28));
		str[1] = TO_HEX(((n & 0x0F000000) >> 24));
		str[2] = TO_HEX(((n & 0x00F00000) >> 20));
		str[3] = TO_HEX(((n & 0x000F0000) >> 16));
		str[4] = TO_HEX(((n & 0x0000F000) >> 12));
		str[5] = TO_HEX(((n & 0x00000F00) >>  8));
		str[6] = TO_HEX(((n & 0x000000F0) >>  4));
		str[7] = TO_HEX(((n & 0x0000000F)));
		str[8] = '\0';
	}

	terminal_writestr(str);
}

void terminal_write_dec(uint32_t n)
{
	char tmp[11];

	itoa(n, tmp, 10);

	terminal_writestr(tmp);
}

void terminal_writestr(const char* data)
{
	terminal_write(data, strlen(data));
}


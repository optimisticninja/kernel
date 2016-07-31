#include <kernel/tty.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct {
	size_t row;
	size_t column;
} VGAPosition;

typedef struct {
	VGAPosition pos;
	VGAColor color;
} VGAEntry;

static VGAPosition cur_pos;
static VGAColor cur_color;
static uint16_t* terminal_buffer;

extern void move_cursor(uint16_t pos);

void terminal_clear()
{
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = get_vga_entry(' ', cur_color);
		}
	}
}

static void scroll()
{
	uint8_t attr_byte = get_color_code(COLOR_BLACK, COLOR_WHITE);
	uint16_t blank = get_vga_entry(' ', attr_byte);
	
	if (cur_pos.row >= 25) {
		unsigned i;
		for (i = 0 * VGA_WIDTH; i < ((VGA_HEIGHT-1) * VGA_WIDTH); i++)
			terminal_buffer[i] = terminal_buffer[i+VGA_WIDTH];
		for (i = (VGA_HEIGHT-1) * VGA_WIDTH; i < (VGA_HEIGHT * VGA_WIDTH); i++)
			terminal_buffer[i] = blank;

		cur_pos.column = 24;
	}
}

void terminal_initialize()
{
	cur_pos.row = 0;
	cur_pos.column = 0;
	cur_color = get_color_code(COLOR_BLACK, COLOR_YELLOW);
	terminal_buffer = VGA_MEMORY;
	terminal_clear();
}
 
void terminal_setcolor(VGAColor color)
{
	cur_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = get_vga_entry(c, color);
}
 
void terminal_putchar(char c)
{
	switch (c) {
		case '\b': {
			if (cur_pos.column)
				cur_pos.column--;
			break;
		}		
		case '\n': {
			cur_pos.column = 0;
			cur_pos.row++;
			break;
		}
		case '\t': {
			cur_pos.column = (cur_pos.column+8) & ~(8-1);
			break;
		}
		case '\r': {
			cur_pos.column = 0;
			break;
		}
		default: {
			if (c >= ' ')
				terminal_putentryat(c, cur_color, cur_pos.column, cur_pos.row);

			cur_pos.column++;

			if (cur_pos.column >= 80) {
				cur_pos.column = 0;
				cur_pos.row++;
			}
			
			//__attribute__((__unused__))register uint16_t i asm("ax") = ((cur_pos.row-1) * VGA_WIDTH) + cur_pos.column;
			move_cursor(((cur_pos.row-1) * VGA_WIDTH) + cur_pos.column);
			scroll();
			break;
		}
	}
	
}
 
void terminal_print(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_puts(const char* data)
{
	terminal_print(data, strlen(data));
}

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
 
#include <kernel/tty.h>
#include <kernel/arch/x86_64/drivers/vga.h>

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

void terminal_clear()
{
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = get_vga_entry(' ', cur_color);
		}
	}
}

void terminal_initialize()
{
	cur_pos.row = 0;
	cur_pos.column = 0;
	cur_color = get_color_code(COLOR_LIGHT_GREY, COLOR_BLACK);
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
	terminal_putentryat(c, cur_color, cur_pos.column, cur_pos.row);
	if (++cur_pos.column == VGA_WIDTH) {
		cur_pos.column = 0;

		if (++cur_pos.row == VGA_HEIGHT)
			cur_pos.row = 0;
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

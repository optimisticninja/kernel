#include <kernel/tty.h>

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

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

#define INT_HEXSTRING_LENGTH ( sizeof(int) * CHAR_BIT / 4 )

/* Defined if I ever support weird architectures. */
static char const HEXDIGITS[0x10] = {
	 '0', '1', '2', '3', '4', '5', '6', '7',
	 '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static void int_to_hexstring(uint64_t value, char result[INT_HEXSTRING_LENGTH+1])
{
	int i;
	result[INT_HEXSTRING_LENGTH] = '\0';

	for (i = INT_HEXSTRING_LENGTH-1; value; i--, value >>= 4) {
		int d  = value & 0xF;
		result[i] = HEXDIGITS[d];
	}

	for ( ; i >= 0; i--)
		result[i] = '0';
}

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

void terminal_printf(const char* format, va_list params)
{
	va_list parameters;
	va_copy(parameters, params);
 
	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;
 
	while (*format != '\0') {
		if (*format != '%') {
		print_c:
			amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			terminal_print(format, amount);
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format;
 
		if (*(++format) == '%')
			goto print_c;
 
		if (rejected_bad_specifier) {
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}
 
		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			terminal_print(&c, sizeof(c));
		} else if (*format == 's') {
			format++;
			const char* s = va_arg(parameters, const char*);
			terminal_print(s, strlen(s));
		} else if (*format == 'd')  {
			format++;
			int i = va_arg(parameters, int);
			char* ia = itoa(i, NULL, 10);
			terminal_print(ia, strlen(ia));
		} else if (*format == 'u') {
			format++;
			unsigned u = va_arg(parameters, unsigned);
			char* ua = itoa(u, NULL, 10);
			terminal_print(ua, strlen(ua));
		} else if (*format == 'X') {
			format++;
			char buf[INT_HEXSTRING_LENGTH + 1];
			int_to_hexstring(va_arg(parameters, uint64_t), buf);
			terminal_print(buf, strlen(buf));
		} else {
			goto incomprehensible_conversion;
		}
	}
 
	va_end(parameters);
}

void terminal_print_hex(uint32_t n)
{
	int tmp;
	char no_zeroes = 1;

	terminal_print("0x", sizeof("0x"));

	for (int i = 28; i >= 0; i -= 4) {
		tmp = (n >> i) & 0xF;
		
		if (tmp == 0 && no_zeroes != 0)
			continue;

		no_zeroes = 0;
		
		if (tmp >= 0xA)
			terminal_putchar(tmp - 0xA + 'a');
		else
			terminal_putchar(tmp + '0');
	}
}


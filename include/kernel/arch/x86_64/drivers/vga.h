#pragma once

#include <stddef.h>
#include <stdint.h>

__attribute__((__unused__)) static uint16_t* VGA_MEMORY = (uint16_t*) 0xB8000;

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

typedef enum {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_YELLOW = 14,
	COLOR_WHITE = 15,
} VGAColor;

static inline uint8_t get_color_code(VGAColor fg, VGAColor bg)
{
	return fg | bg << 4;
}

static inline uint16_t get_vga_entry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}



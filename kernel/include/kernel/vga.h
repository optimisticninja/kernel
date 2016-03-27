#pragma once

#include <stddef.h>
#include <stdint.h>

#include <kernel/io.h>

typedef enum {
	COLOR_BLACK,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_MAGENTA,
	COLOR_BROWN,
	COLOR_LIGHT_GREY,
	COLOR_DARK_GREY,
	COLOR_LIGHT_BLUE,
	COLOR_LIGHT_GREEN,
	COLOR_LIGHT_CYAN,
	COLOR_LIGHT_RED,
	COLOR_LIGHT_MAGENTA,
	COLOR_YELLOW,
	COLOR_WHITE
} VGAColor;

static uint16_t* VGA_MEM_START = (uint16_t*) 0xB8000;

static const size_t VGA_WIDTH = 80,
					VGA_HEIGHT = 25;


static inline uint8_t make_color(VGAColor fg, VGAColor bg)
{
	return fg | bg << 4;
}
 
static inline uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}


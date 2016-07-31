#pragma once

#include <stddef.h>

#include <kernel/arch/x86_64/drivers/vga.h>

void terminal_initialize();
void terminal_clear();
void terminal_putchar(char c);
void terminal_print(const char* data, size_t size);
void terminal_puts(const char* data);
void terminal_setcolor(VGAColor color);


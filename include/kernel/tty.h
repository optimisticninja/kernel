#pragma once

#include <stddef.h>
#include <stdarg.h>

#include <kernel/arch/x86_64/drivers/vga.h>

void terminal_initialize();
void terminal_clear();
void terminal_putchar(char c);
void terminal_print(const char* data, size_t size);
void terminal_puts(const char* data);
void terminal_setcolor(VGAColor color);
void terminal_print_hex(uint32_t n);
void terminal_printf(const char* format, va_list params);


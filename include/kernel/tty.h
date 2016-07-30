#pragma once

#include <stddef.h>

#include <kernel/arch/x86_64/drivers/vga.h>

void terminal_initialize();
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);


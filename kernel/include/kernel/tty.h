#pragma once

#include <stddef.h>
#include <stdint.h>

void terminal_initialize();
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_write_hex(uint32_t n);
void terminal_write_dec(uint32_t n);
void terminal_writestr(const char* data);


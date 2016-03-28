#pragma once

#include <stdint.h>

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char* msg, const char* file, uint32_t line);
extern void panic_assert(const char* file, uint32_t line, const char* desc);
uint32_t kmalloc_int(uint32_t sz, int align, uint32_t* phys);
uint32_t kmalloc_a(uint32_t sz);
uint32_t kmalloc_p(uint32_t sz, uint32_t* phys);
uint32_t kmalloc_ap(uint32_t sz, uint32_t* phys);
uint32_t kmalloc(uint32_t sz);


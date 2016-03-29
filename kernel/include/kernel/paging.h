#pragma once

#include <stdint.h>
#include <kernel/isr.h>

typedef struct {
    uint32_t present    : 1;   // Page present in memory
    uint32_t rw         : 1;   // Read-only if clear, readwrite if set
    uint32_t user       : 1;   // Supervisor level only if clear
    uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
    uint32_t dirty      : 1;   // Has the page been written to since last refresh?
    uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
    uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} Page;

typedef struct {
    Page pages[1024];
} PageTable;

typedef struct page_directory
{
    PageTable* tables[1024];
    uint32_t tables_physical[1024];
    uint32_t physical_addr;
} PageDirectory;

void init_paging();
void switch_page_directory(PageDirectory* new);
Page* get_page(uint32_t address, int make, PageDirectory* dir);
void page_fault(Registers* regs);


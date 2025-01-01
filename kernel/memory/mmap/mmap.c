#include "mmap.h"

uint16_t entry_count;
mmap_entry_t *entries;
mmap_entry_t *first_usable_entry;
uint64_t total_memory;
uint32_t kernel_start;
unsigned long kernel_end;
uint64_t last_available_address;
uint16_t i;

void parse_mmap(uint32_t k_start) {
    kernel_start = k_start;
    kernel_end = (unsigned long)&end_of_kernel;
    entry_count = *((uint16_t*)0x9000);
    entries = (mmap_entry_t*)0x9004;

    if (entry_count <= 0)
        kpanic(0);
        
    total_memory = 0;
    last_available_address = 0;

    for (i = 0; i < entry_count; i++) {
        mmap_entry_t *entry = (mmap_entry_t*)((uint8_t*)entries + i * sizeof(mmap_entry_t));

        if (entry->type != 1)
            continue;
        else if (first_usable_entry == NULL)
            first_usable_entry = entry;
        
        uint64_t base = ((uint64_t)entry->base_high << 32) | entry->base_low;
        uint64_t length = ((uint64_t)entry->length_high << 32) | entry->length_low;

        total_memory += length;

        uint64_t last_address = base + length - 1;

        if (last_address > last_available_address)
            last_available_address = last_address;
    }

    if (!first_usable_entry)
        kpanic(0);

    uint64_t base = ((uint64_t)first_usable_entry->base_high << 32) | first_usable_entry->base_low;
    uint64_t length = ((uint64_t)first_usable_entry->length_high << 32) | first_usable_entry->length_low;

    kprint("[ ");
    term_setcolor(VGA_COLOR_BLACK, VGA_COLOR_BROWN);
    kprint("KERNEL");
    term_setcolor(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    kprint(" ] memory map... ");
    term_setcolor(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREEN);
    kprint("OK\n");
    term_setcolor(VGA_COLOR_BLACK, VGA_COLOR_WHITE);

    if (debug)
        kprint("    total available memory: %.1f GB (%lld bytes)\n    kernel start: 0x%lx\n    kernel end: 0x%lx\n    kernel size: %d bytes\n    last available address: 0x%lx\n", (double)total_memory / 1000000000.0, total_memory, kernel_start, kernel_end, kernel_end - kernel_start, last_available_address);
}
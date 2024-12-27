#include "mmap.h"

uint16_t entry_count;
mmap_entry_t *entries;
mmap_entry_t *first_usable_entry;
uint32_t total_memory;
unsigned long kernel_end;
uint16_t i;

void parse_mmap() {
    kernel_end = (unsigned long)&end_of_kernel;
    entry_count = *((uint16_t*)0x8240);
    entries = (mmap_entry_t*)0x8244;

    if (entry_count <= 0)
        kpanic(0);
        
    total_memory = 0;

    for (i = 0; i < entry_count; i++) {
        mmap_entry_t *entry = (mmap_entry_t*)((uint8_t*)entries + i * sizeof(mmap_entry_t));

        if (entry->type != 1)
            continue;
        else if (first_usable_entry == NULL)
            first_usable_entry = entry;
        
        uint64_t length = ((uint64_t)entry->length_high << 32) | entry->length_low;

        total_memory += length;
    }

    if (!first_usable_entry)
        kpanic(0);

    uint64_t base = ((uint64_t)first_usable_entry->base_high << 32) | first_usable_entry->base_low;
    uint64_t length = ((uint64_t)first_usable_entry->length_high << 32) | first_usable_entry->length_low;

    printf("memory map... [OK]\n");

    if (debug)
        printf("    total available memory: %.1f GB\n    end of kernel: 0x%lx\n", (double)total_memory / 1073741824.0, kernel_end);
}
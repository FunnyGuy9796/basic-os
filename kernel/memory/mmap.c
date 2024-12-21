#include "mmap.h"

uint16_t entry_count;
mmap_entry *entries;

void parse_mmap() {
    entry_count = *((uint16_t*)0x4000);
    entries = (mmap_entry*)0x4004;

    printf("\nfound %d entries in the memory map...\n", entry_count);

    for (uint16_t i = 0; i < entry_count; i++) {
        mmap_entry* entry = &entries[i];

        printf("region %d: { Base: 0x%lx, Length: 0x%lx, Type: %d }\n", i + 1, entry->base_addr, entry->length, entry->type);
    }
}
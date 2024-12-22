#include "mmap.h"

uint16_t entry_count;
mmap_entry* entries;

void parse_mmap() {
    entry_count = *((uint16_t*)0x8100);
    entries = (mmap_entry*)0x8104;

    printf("\nfound %d entries in the memory map...\n", entry_count);

    for (uint16_t i = 0; i < entry_count; i++) {
        mmap_entry* entry = (mmap_entry*)((uint8_t*)entries + i * sizeof(mmap_entry));
        uint64_t base = ((uint64_t)entry->base_high << 32) | entry->base_low;
        uint64_t length = ((uint64_t)entry->length_high << 32) | entry->length_low;

        printf("Region %d: ", i + 1);
        printf(" Base: 0x%llx", base);
        printf(" Length: 0x%llx", length);
        printf(" Type: %d", entry->type);
        printf(" ACPI: 0x%x\n", entry->acpi);
    }
}